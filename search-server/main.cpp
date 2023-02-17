#include <algorithm>
#include <numeric>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

struct Document {
    int id;
    double relevance;
    int rating;
};

enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};

class SearchServer {
public:
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }

    void AddDocument(int document_id, const string& document, DocumentStatus status,
                     const vector<int>& ratings) {
        const vector<string> words = SplitIntoWordsNoStop(document);
        const double inv_word_count = 1.0 / words.size();
        for (const string& word : words) {
            word_to_document_freqs_[word][document_id] += inv_word_count;
        }
        documents_.emplace(document_id, DocumentData{ComputeAverageRating(ratings), status});
    }

    template <typename DocumentPredicate>
    vector<Document> FindTopDocuments(const string& raw_query, DocumentPredicate document_predicate) const {
        const Query query = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query, document_predicate);
        const double EPSILON = 1e-6;

        sort(matched_documents.begin(), matched_documents.end(),
             [EPSILON](const Document& lhs, const Document& rhs) {
                 if (abs(lhs.relevance - rhs.relevance) < EPSILON) {
                     return lhs.rating > rhs.rating;
                 } else {
                     return lhs.relevance > rhs.relevance;
                 }
             });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

    vector<Document> FindTopDocuments(const string& raw_query, DocumentStatus status) const {
        return FindTopDocuments(raw_query,
            [status](int document_id, DocumentStatus document_status, int rating) {
                return document_status == status;
            });
    }

    vector<Document> FindTopDocuments(const string& raw_query) const {
        return FindTopDocuments(raw_query, DocumentStatus::ACTUAL);
    }

    int GetDocumentCount() const {
        return documents_.size();
    }

    tuple<vector<string>, DocumentStatus> MatchDocument(const string& raw_query,
                                                        int document_id) const {
        const Query query = ParseQuery(raw_query);
        vector<string> matched_words;
        for (const string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            if (word_to_document_freqs_.at(word).count(document_id)) {
                matched_words.push_back(word);
            }
        }
        for (const string& word : query.minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            if (word_to_document_freqs_.at(word).count(document_id)) {
                matched_words.clear();
                break;
            }
        }
        return {matched_words, documents_.at(document_id).status};
    }

private:
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };

    set<string> stop_words_;
    map<string, map<int, double>> word_to_document_freqs_;
    map<int, DocumentData> documents_;

    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }

    static int ComputeAverageRating(const vector<int>& ratings) {
        if (ratings.empty()) {
            return 0;
        }
        int rating_sum = accumulate(ratings.begin(), ratings.end(), 0);
        return rating_sum / static_cast<int>(ratings.size());
    }

    struct QueryWord {
        string data;
        bool is_minus;
        bool is_stop;
    };

    QueryWord ParseQueryWord(string text) const {
        bool is_minus = false;
        // Word shouldn't be empty
        if (text[0] == '-') {
            is_minus = true;
            text = text.substr(1);
        }
        return {text, is_minus, IsStopWord(text)};
    }

    struct Query {
        set<string> plus_words;
        set<string> minus_words;
    };

    Query ParseQuery(const string& text) const {
        Query query;
        for (const string& word : SplitIntoWords(text)) {
            const QueryWord query_word = ParseQueryWord(word);
            if (!query_word.is_stop) {
                if (query_word.is_minus) {
                    query.minus_words.insert(query_word.data);
                } else {
                    query.plus_words.insert(query_word.data);
                }
            }
        }
        return query;
    }

    // Existence required
    double ComputeWordInverseDocumentFreq(const string& word) const {
        return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
    }

    template <typename DocumentPredicate>
    vector<Document> FindAllDocuments(const Query& query, DocumentPredicate document_predicate) const {
        map<int, double> document_to_relevance;
        for (const string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
            for (const auto [document_id, term_freq] : word_to_document_freqs_.at(word)) {
                const auto& document_data = documents_.at(document_id);
                if (document_predicate(document_id, document_data.status, document_data.rating)) {
                    document_to_relevance[document_id] += term_freq * inverse_document_freq;
                }
            }
        }

        for (const string& word : query.minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            for (const auto [document_id, _] : word_to_document_freqs_.at(word)) {
                document_to_relevance.erase(document_id);
            }
        }

        vector<Document> matched_documents;
        for (const auto [document_id, relevance] : document_to_relevance) {
            matched_documents.push_back(
                {document_id, relevance, documents_.at(document_id).rating});
        }
        return matched_documents;
    }
};

// -------- Начало модульных тестов поисковой системы ----------

template <typename First, typename Second>
ostream& operator<<(ostream& out, const pair<First, Second>& p) {
    return out << p.first << ": "s << p.second;
}

template <typename Container>
void Print(ostream& out, const Container& container) {
    bool is_first = true;
    for (const auto& element : container) {
        if (!is_first) {
            out << ", "s;
        }
        is_first = false;
        out << element;
    }
}

template <typename Element>
ostream& operator<<(ostream& out, const vector<Element>& container) {
    out << '[';
    Print(out, container);
    out << ']';
    return out;
}

template <typename Element>
ostream& operator<<(ostream& out, const set<Element>& container) {
    out << '{';
    Print(out, container);
    out << '}';
    return out;
}

template <typename Key, typename Value>
ostream& operator<<(ostream& out, const map<Key, Value>& container) {
    out << '{';
    Print(out, container);
    out << '}';
    return out;
}

template <typename T, typename U>
void AssertEqualImpl(const T& t, const U& u, const string& t_str, const string& u_str, const string& file,
                     const string& func, unsigned line, const string& hint) {
    if (t != u) {
        cerr << boolalpha;
        cerr << file << "("s << line << "): "s << func << ": "s;
        cerr << "ASSERT_EQUAL("s << t_str << ", "s << u_str << ") failed: "s;
        cerr << t << " != "s << u << "."s;
        if (!hint.empty()) {
            cerr << " Hint: "s << hint;
        }
        cerr << endl;
        abort();
    }
}

#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

void AssertImpl(bool value, const string& expr_str, const string& file, const string& func, unsigned line,
                const string& hint) {
    if (!value) {
        cerr << file << "("s << line << "): "s << func << ": "s;
        cerr << "ASSERT("s << expr_str << ") failed."s;
        if (!hint.empty()) {
            cerr << " Hint: "s << hint;
        }
        cerr << endl;
        abort();
    }
}

#define ASSERT(expr) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_HINT(expr, hint) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, (hint))

template <typename T>
void RunTestImpl(T& func, const string& func_name) {
    func();
    cerr << func_name << " OK" << endl;
}

#define RUN_TEST(func) RunTestImpl((func), #func)


void TestAddDocument() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};

    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("found a cat"s);
        ASSERT_EQUAL(found_docs.size(), 1u);
        const Document& doc0 = found_docs[0];
        ASSERT_EQUAL_HINT(doc0.id, doc_id,
                            "The document must be added"s);
    }
}

void TestExcludeStopWordsFromAddedDocumentContent() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in"s);
        ASSERT_EQUAL(found_docs.size(), 1u);
        const Document& doc0 = found_docs[0];
        ASSERT_EQUAL(doc0.id, doc_id);
    }

    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        ASSERT_HINT(server.FindTopDocuments("in"s).empty(),
                    "Stop words must be excluded from documents"s);
    }
}

void TestExcludeMinusWordsFromAddedDocumentContent() {
    const int doc_id = 42;
    const string content = "found a cat with a collar"s;
    const vector<int> ratings = {1, 2, 3};

    const int other_doc_id = 43;
    const string other_content = "there was a cat with a collar and a tracker"s;
    const vector<int> other_ratings = {7, 2, 7};

    {
        SearchServer server;
        server.SetStopWords("a with and the here is"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        server.AddDocument(other_doc_id, other_content, DocumentStatus::ACTUAL, other_ratings);
        ASSERT_HINT(server.FindTopDocuments("the cat here is the -cat"s).empty(),
                    "The same word is excluded if it is in the query with a minus and without"s);
    }

    {
        SearchServer server;
        server.SetStopWords("a with and"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        server.AddDocument(other_doc_id, other_content, DocumentStatus::ACTUAL, other_ratings);
        const auto found_docs = server.FindTopDocuments("a cat with a collar -tracker"s);
        ASSERT_EQUAL(found_docs.size(), 1u);
        const Document& doc0 = found_docs[0];
        ASSERT_EQUAL_HINT(doc0.id, doc_id,
                            "Words with a minus exclude documents from the search results"s);
    }
}

void TestExcludeMinusWordsAddedMatchDocument() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};

    const int other_doc_id = 43;
    const string other_content = "a cat on the street"s;
    const vector<int> other_ratings = {7, 2, 7};

    SearchServer server;
    server.SetStopWords("a in the on"s);
    server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
    server.AddDocument(other_doc_id, other_content, DocumentStatus::ACTUAL, other_ratings);

    {
        const auto [matched_words, status] = server.MatchDocument("found a cat in the city"s, doc_id);
        const vector<string> found_words = {"cat"s, "city"s};
        ASSERT_EQUAL_HINT(matched_words, found_words,
                            "All the words from the query present in the document must be returned"s);
    }

    {
        const auto [matched_words, status] = server.MatchDocument("found a cat in the -city"s, other_doc_id);
        const vector<string> found_words = {"cat"s};
        ASSERT_EQUAL_HINT(matched_words, found_words,
                            "Words with a minus in the query should be excluded"s);
    }

    {
        const auto [matched_words, status] = server.MatchDocument("found in the -city"s, doc_id);
        const vector<string> found_words;
        ASSERT_EQUAL_HINT(matched_words, found_words,
                            "If there is a match for words with a minus, an empty list of words should be returned in the request"s);
    }
}

void TestSortingFoundDocumentsByRelevance() {
    const int doc0_id = 41;
    const string doc0_content = "a white cat and a fashionable collar"s;
    const vector<int> doc0_ratings = {8, -3};

    const int doc1_id = 42;
    const string doc1_content = "fluffy cat fluffy tail"s;
    const vector<int> doc1_ratings = {7, 2, 7};

    const int doc2_id = 43;
    const string doc2_content = "beautiful dog expressive eyes"s;
    const vector<int> doc2_ratings = {5, -12, 2, 1};

    const int doc3_id = 44;
    const string doc3_content = "beautiful starling eugene"s;
    const vector<int> doc3_ratings = {9};

    const double EPSILON = 1e-6;

    {
        SearchServer server;
        server.SetStopWords("a and"s);
        server.AddDocument(doc0_id, doc0_content, DocumentStatus::ACTUAL, doc0_ratings);
        server.AddDocument(doc1_id, doc1_content, DocumentStatus::ACTUAL, doc1_ratings);
        server.AddDocument(doc2_id, doc2_content, DocumentStatus::ACTUAL, doc2_ratings);
        server.AddDocument(doc3_id, doc3_content, DocumentStatus::ACTUAL, doc3_ratings);
        const auto found_docs = server.FindTopDocuments("fluffy beautiful cat"s);
        bool first_relevance = true;
        bool first_rating = true;
        bool first = true;
        int j = 1;

        for (auto i = 0; i != found_docs.size(); ++i) {
            if (abs(found_docs[i].relevance - found_docs[i + j].relevance) < EPSILON) {
                if (first_rating) {
                    ASSERT_EQUAL_HINT(found_docs[i].id, doc0_id,
                                        "If the relevance values are approximately equal, the returned results should be sorted in order of rating"s);
                    first_rating = false;
                } else {
                    ASSERT_EQUAL_HINT(found_docs[i].id, doc2_id,
                                        "If the relevance values are approximately equal, the returned results should be sorted in order of rating"s);
                }
            } else {
                if (first_relevance) {
                    ASSERT_EQUAL_HINT(found_docs[i].id, doc1_id,
                                        "The returned results should be sorted in descending order of relevance"s);
                    first_relevance = false;
                } else {
                    ASSERT_EQUAL_HINT(found_docs[i].id, doc3_id,
                                        "The returned results should be sorted in descending order of relevance"s);
                    if (first) {
                        first = false;
                        j = 0;
                    }
                }
            }
        }
    }
}

void TestComputeRatingDocument() {
    const int doc0_id = 42;
    const string doc0_content = "cat in the city"s;
    const vector<int> doc0_ratings = {8, -3};

    const int doc1_id = 43;
    const string doc1_content = "a cat on the street"s;
    const vector<int> doc1_ratings = {9};

    const int doc2_id = 44;
    const string doc2_content = "the cat in the yard"s;
    const vector<int> doc2_ratings = {5, -12, 2, 1};

    {
        SearchServer server;
        server.SetStopWords("a in the on"s);
        server.AddDocument(doc0_id, doc0_content, DocumentStatus::ACTUAL, doc0_ratings);
        server.AddDocument(doc1_id, doc1_content, DocumentStatus::ACTUAL, doc1_ratings);
        server.AddDocument(doc2_id, doc2_content, DocumentStatus::ACTUAL, doc2_ratings);
        const auto found_docs = server.FindTopDocuments("found a cat in the city"s);
        const int doc0_id_42_rating = found_docs[0].rating;
        const int doc1_id_43_rating = found_docs[1].rating;
        const int doc2_id_44_rating = found_docs[2].rating;
        ASSERT_EQUAL_HINT(doc0_id_42_rating, 2,
                            "The rating of the added document is equal to the arithmetic mean of the document ratings"s);
        ASSERT_EQUAL_HINT(doc1_id_43_rating, 9,
                            "The rating of the added document is equal to the arithmetic mean of the document ratings"s);
        ASSERT_EQUAL_HINT(doc2_id_44_rating, -1,
                            "The rating of the added document is equal to the arithmetic mean of the document ratings"s);
    }
}

void TestFilteringResultsUsingByPredicate() {
    const int doc0_id = 42;
    const string doc0_content = "cat in the city"s;
    const vector<int> doc0_ratings = {8, -3};

    const int doc1_id = 43;
    const string doc1_content = "a cat on the street"s;
    const vector<int> doc1_ratings = {9};

    const int doc2_id = 44;
    const string doc2_content = "the cat in the yard"s;
    const vector<int> doc2_ratings = {5, -12, 2, 1};

    {
        SearchServer server;
        server.SetStopWords("a in the on"s);
        server.AddDocument(doc0_id, doc0_content, DocumentStatus::ACTUAL, doc0_ratings);
        server.AddDocument(doc1_id, doc1_content, DocumentStatus::ACTUAL, doc1_ratings);
        server.AddDocument(doc2_id, doc2_content, DocumentStatus::ACTUAL, doc2_ratings);
        const auto found_docs = server.FindTopDocuments("a cat"s,
            [](int document_id, DocumentStatus status, int rating) { return document_id % 2 == 0; });
        ASSERT_EQUAL_HINT(found_docs[0].id, doc0_id,
                            "Filtering the results should be using a predicate set by the user"s);
        ASSERT_EQUAL_HINT(found_docs[1].id, doc2_id,
                            "Filtering the results should be using a predicate set by the user"s);
    }

    {
        SearchServer server;
        server.SetStopWords("a in the on"s);
        server.AddDocument(doc0_id, doc0_content, DocumentStatus::ACTUAL, doc0_ratings);
        server.AddDocument(doc1_id, doc1_content, DocumentStatus::ACTUAL, doc1_ratings);
        server.AddDocument(doc2_id, doc2_content, DocumentStatus::ACTUAL, doc2_ratings);
        const auto found_docs = server.FindTopDocuments("a cat"s,
            [](int document_id, DocumentStatus status, int rating) { return rating > 0; });
        ASSERT_EQUAL_HINT(found_docs[0].id, doc1_id,
                            "Filtering the results should be using a predicate set by the user"s);
        ASSERT_EQUAL_HINT(found_docs[1].id, doc0_id,
                            "Filtering the results should be using a predicate set by the user"s);
    }

    {
        SearchServer server;
        server.SetStopWords("a in the on"s);
        server.AddDocument(doc0_id, doc0_content, DocumentStatus::ACTUAL, doc0_ratings);
        server.AddDocument(doc1_id, doc1_content, DocumentStatus::REMOVED, doc1_ratings);
        server.AddDocument(doc2_id, doc2_content, DocumentStatus::REMOVED, doc2_ratings);
        const auto found_docs = server.FindTopDocuments("a cat"s,
            [](int document_id, DocumentStatus status, int rating) { return status == DocumentStatus::ACTUAL; });
        ASSERT_EQUAL_HINT(found_docs[0].id, doc0_id,
                            "Filtering the results should be using a predicate set by the user"s);
    }
}

void TestSearchDocumentsByStatus() {
    const int doc0_id = 42;
    const string doc0_content = "cat in the city"s;
    const vector<int> doc0_ratings = {8, -3};

    const int doc1_id = 43;
    const string doc1_content = "a cat on the street"s;
    const vector<int> doc1_ratings = {9};

    const int doc2_id = 44;
    const string doc2_content = "the cat in the yard"s;
    const vector<int> doc2_ratings = {5, -12, 2, 1};

    {
        SearchServer server;
        server.SetStopWords("a in the on"s);
        server.AddDocument(doc0_id, doc0_content, DocumentStatus::ACTUAL, doc0_ratings);
        server.AddDocument(doc1_id, doc1_content, DocumentStatus::BANNED, doc1_ratings);
        server.AddDocument(doc2_id, doc2_content, DocumentStatus::REMOVED, doc2_ratings);
        const auto found_docs = server.FindTopDocuments("a cat"s, DocumentStatus::ACTUAL);
        ASSERT_EQUAL_HINT(found_docs[0].id, doc0_id,
                            "Filtering of document search results should be based on the specified status"s);
    }
}

void TestComputeRelevanceOfFoundDocuments() {
    const int doc0_id = 41;
    const string doc0_content = "a white cat and a fashionable collar"s;
    const vector<int> doc0_ratings = {8, -3};

    const int doc1_id = 42;
    const string doc1_content = "fluffy cat fluffy tail"s;
    const vector<int> doc1_ratings = {7, 2, 7};

    const int doc2_id = 43;
    const string doc2_content = "beautiful dog expressive eyes"s;
    const vector<int> doc2_ratings = {5, -12, 2, 1};

    const int doc3_id = 44;
    const string doc3_content = "beautiful starling eugene"s;
    const vector<int> doc3_ratings = {9};

    const double EPSILON = 1e-6;

    {
        SearchServer server;
        server.SetStopWords("a and"s);
        server.AddDocument(doc0_id, doc0_content, DocumentStatus::ACTUAL, doc0_ratings);
        server.AddDocument(doc1_id, doc1_content, DocumentStatus::ACTUAL, doc1_ratings);
        server.AddDocument(doc2_id, doc2_content, DocumentStatus::ACTUAL, doc2_ratings);
        server.AddDocument(doc3_id, doc3_content, DocumentStatus::ACTUAL, doc3_ratings);
        const auto found_docs = server.FindTopDocuments("fluffy beautiful cat"s);
        const double doc1_absolute_value = abs(found_docs[0].relevance - 0.866434);
        const double doc3_absolute_value = abs(found_docs[1].relevance - 0.231049);
        const double doc0_absolute_value = abs(found_docs[2].relevance - 0.173287);
        const double doc2_absolute_value = abs(found_docs[3].relevance - 0.173287);
        ASSERT_HINT(doc1_absolute_value < EPSILON,
                    "The difference in calculating the relevance of the found documents should fit into the error (EPSILON)"s);
        ASSERT_HINT(doc3_absolute_value < EPSILON,
                    "The difference in calculating the relevance of the found documents should fit into the error (EPSILON)"s);
        ASSERT_HINT(doc0_absolute_value < EPSILON,
                    "The difference in calculating the relevance of the found documents should fit into the error (EPSILON)"s);
        ASSERT_HINT(doc2_absolute_value < EPSILON,
                    "The difference in calculating the relevance of the found documents should fit into the error (EPSILON)"s);
    }

}

// Функция TestSearchServer является точкой входа для запуска тестов
void TestSearchServer() {
    RUN_TEST(TestAddDocument);
    RUN_TEST(TestExcludeStopWordsFromAddedDocumentContent);
    RUN_TEST(TestExcludeMinusWordsFromAddedDocumentContent);
    RUN_TEST(TestExcludeMinusWordsAddedMatchDocument);
    RUN_TEST(TestSortingFoundDocumentsByRelevance);
    RUN_TEST(TestComputeRatingDocument);
    RUN_TEST(TestFilteringResultsUsingByPredicate);
    RUN_TEST(TestSearchDocumentsByStatus);
    RUN_TEST(TestComputeRelevanceOfFoundDocuments);
}

// --------- Окончание модульных тестов поисковой системы -----------

// ==================== для примера =========================

void PrintDocument(const Document& document) {
    cout << "{ "s
         << "document_id = "s << document.id << ", "s
         << "relevance = "s << document.relevance << ", "s
         << "rating = "s << document.rating
         << " }"s << endl;
}

int main() {
    TestSearchServer();
    // Если вы видите эту строку, значит все тесты прошли успешно
    cout << "Search server testing finished"s << endl;

    SearchServer search_server;
    search_server.SetStopWords("и в на"s);
    search_server.AddDocument(0, "белый кот и модный ошейник"s,        DocumentStatus::ACTUAL, {8, -3});
    search_server.AddDocument(1, "пушистый кот пушистый хвост"s,       DocumentStatus::ACTUAL, {7, 2, 7});
    search_server.AddDocument(2, "ухоженный пёс выразительные глаза"s, DocumentStatus::ACTUAL, {5, -12, 2, 1});
    search_server.AddDocument(3, "ухоженный скворец евгений"s,         DocumentStatus::BANNED, {9});
    cout << "ACTUAL by default:"s << endl;
    for (const Document& document : search_server.FindTopDocuments("пушистый ухоженный кот"s)) {
        PrintDocument(document);
    }
    cout << "BANNED:"s << endl;
    for (const Document& document : search_server.FindTopDocuments("пушистый ухоженный кот"s, DocumentStatus::BANNED)) {
        PrintDocument(document);
    }
    cout << "Even ids:"s << endl;
    for (const Document& document : search_server.FindTopDocuments("пушистый ухоженный кот"s, [](int document_id, DocumentStatus status, int rating) { return document_id % 2 == 0; })) {
        PrintDocument(document);
    }
    return 0;
}