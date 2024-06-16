#include "search_server.h"

SearchServer::SearchServer(const std::string& stop_words_text)
    : SearchServer(std::string_view(stop_words_text))
{
}

SearchServer::SearchServer(std::string_view stop_words_text)
    : SearchServer(SplitIntoWords(stop_words_text))
{
}

void SearchServer::AddDocument(int document_id, std::string_view document, DocumentStatus status,
                     const std::vector<int>& ratings) {
    if ((document_id < 0) || (documents_.count(document_id) > 0)) {
        throw std::invalid_argument("Invalid document_id");
    }
    buffer_.emplace_back(document);
    std::vector<std::string_view> words;
    if (!buffer_.empty()){
        words = SplitIntoWordsNoStop(buffer_.back());
    }
    const double inv_word_count = 1.0 / words.size();
    for (std::string_view word : words) {
        word_to_document_freqs_[word][document_id] += inv_word_count;
        freqs_of_document_words_[document_id][word] += inv_word_count;
    }
    documents_.emplace(document_id, DocumentData{ComputeAverageRating(ratings), status});
    document_ids_.insert(document_id);
}

std::vector<Document> SearchServer::FindTopDocuments(std::string_view raw_query, DocumentStatus status) const {
    return SearchServer::FindTopDocuments(std::execution::seq, raw_query, status);
}

std::vector<Document> SearchServer::FindTopDocuments(const std::execution::sequenced_policy&, std::string_view raw_query, DocumentStatus status) const {
    return SearchServer::FindTopDocuments(
        raw_query, [status](int document_id, DocumentStatus document_status, int rating) {
            return document_status == status;
        });
}

std::vector<Document> SearchServer::FindTopDocuments(const std::execution::parallel_policy&, std::string_view raw_query, DocumentStatus status) const {
    return SearchServer::FindTopDocuments(
        std::execution::par,
        raw_query, [status](int document_id, DocumentStatus document_status, int rating) {
            return document_status == status;
        });
}

std::vector<Document> SearchServer::FindTopDocuments(std::string_view raw_query) const {
    return SearchServer::FindTopDocuments(std::execution::seq, raw_query);
}

std::vector<Document> SearchServer::FindTopDocuments(const std::execution::sequenced_policy&, std::string_view raw_query) const {
    return SearchServer::FindTopDocuments(raw_query, DocumentStatus::ACTUAL);
}

std::vector<Document> SearchServer::FindTopDocuments(const std::execution::parallel_policy&, std::string_view raw_query) const {
    return SearchServer::FindTopDocuments(std::execution::par, raw_query, DocumentStatus::ACTUAL);
}

int SearchServer::GetDocumentCount() const {
    return documents_.size();
}

std::set<int>::const_iterator SearchServer::begin() const {
    return document_ids_.begin();
}
std::set<int>::const_iterator SearchServer::end() const {
    return document_ids_.end();
}

const std::map<std::string_view, double>& SearchServer::GetWordFrequencies(int document_id) const {
    static const std::map<std::string_view, double> empty_map;
    if (freqs_of_document_words_.find(document_id) != freqs_of_document_words_.end()) {
        return freqs_of_document_words_.at(document_id);
    }
    return empty_map;
}

void SearchServer::RemoveDocument(int document_id) {
    SearchServer::RemoveDocument(std::execution::seq, document_id);
}

void SearchServer::RemoveDocument(const std::execution::sequenced_policy&, int document_id) {
    if (documents_.count(document_id) == 0) return;

    for (const auto& [word, freq] : GetWordFrequencies(document_id)) {
        word_to_document_freqs_[word].erase(document_id);
    }
    documents_.erase(document_id);
    document_ids_.erase(document_id);
    freqs_of_document_words_.erase(document_id);
}

void SearchServer::RemoveDocument(const std::execution::parallel_policy&, int document_id) {
    if (documents_.count(document_id) == 0) return;

    const auto& words_freqs = GetWordFrequencies(document_id);
    std::vector<std::string_view> words(words_freqs.size());
    std::transform(
        std::execution::par,
        words_freqs.begin(), words_freqs.end(),
        words.begin(),
        [](const auto& item) {
            return item.first;
    });
    std::for_each(
        std::execution::par,
        words.begin(), words.end(),
        [this, document_id](std::string_view word) {
            word_to_document_freqs_[word].erase(document_id);
    });

    documents_.erase(document_id);
    document_ids_.erase(document_id);
    freqs_of_document_words_.erase(document_id);
}

std::tuple<std::vector<std::string_view>, DocumentStatus> SearchServer::MatchDocument(std::string_view raw_query,
                                                        int document_id) const {
    return SearchServer::MatchDocument(std::execution::seq, raw_query, document_id);
}

std::tuple<std::vector<std::string_view>, DocumentStatus> SearchServer::MatchDocument(const std::execution::sequenced_policy&,
                                                                                    std::string_view raw_query, int document_id) const {
    if (!IsValidWord(raw_query)) {
        throw std::invalid_argument("Invalid query");
    }
    if (document_id < 0 || documents_.count(document_id) == 0) {
        throw std::out_of_range("Invalid document_id");
    }

    const auto query = ParseQuery(raw_query, true);
    std::vector<std::string_view> matched_words;
    for (const std::string_view& word : query.minus_words) {
        if (word_to_document_freqs_.count(word) == 0) {
            continue;
        }
        if (word_to_document_freqs_.at(word).count(document_id)) {
            return std::make_tuple(std::vector<std::string_view>{}, documents_.at(document_id).status);
        }
    }
    for (const std::string_view& word : query.plus_words) {
        if (word_to_document_freqs_.count(word) == 0) {
            continue;
        }
        if (word_to_document_freqs_.at(word).count(document_id)) {
            matched_words.push_back(word);
        }
    }
    return {matched_words, documents_.at(document_id).status};
}
std::tuple<std::vector<std::string_view>, DocumentStatus> SearchServer::MatchDocument(const std::execution::parallel_policy&,
                                                                                    std::string_view raw_query, int document_id)  const {
    if (!IsValidWord(raw_query)) {
        throw std::invalid_argument("Invalid query");
    }
    if (document_id < 0 || documents_.count(document_id) == 0) {
        throw std::out_of_range("Invalid document_id");
    }

    const auto query = ParseQuery(raw_query);
    const auto& words_freqs = GetWordFrequencies(document_id);
    if (std::any_of(
                    std::execution::par,
                    query.minus_words.begin(), query.minus_words.end(),
                    [&words_freqs](std::string_view word) { return words_freqs.count(word); }))
        return std::make_tuple(std::vector<std::string_view>{}, documents_.at(document_id).status);

    std::vector<std::string_view> matched_words(query.plus_words.size());
    auto it_end = std::copy_if(
                std::execution::par,
                query.plus_words.begin(), query.plus_words.end(),
                matched_words.begin(),
                [&words_freqs](std::string_view word) { return words_freqs.count(word); });

    std::sort(matched_words.begin(), it_end);
    auto last = std::unique(matched_words.begin(), it_end);
    matched_words.erase(last, matched_words.end());

    return { matched_words, documents_.at(document_id).status };
}

bool SearchServer::IsStopWord(std::string_view word) const {
    return stop_words_.count(word) > 0;
}

bool SearchServer::IsValidWord(std::string_view word) {
    return std::none_of(word.begin(), word.end(), [](char c) {
        return c >= '\0' && c < ' ';
    });
}

std::vector<std::string_view> SearchServer::SplitIntoWordsNoStop(std::string_view text) const {
    std::vector<std::string_view> words;
    for (const std::string_view& word : SplitIntoWords(text)) {
        if (!IsValidWord(word)) {
            throw std::invalid_argument("Word "s + static_cast<std::string>(word) + " is invalid"s);
        }
        if (!IsStopWord(word)) {
            words.push_back(word);
        }
    }
    return words;
}

int SearchServer::ComputeAverageRating(const std::vector<int>& ratings) {
    if (ratings.empty()) {
        return 0;
    }
    int rating_sum = std::accumulate(ratings.begin(), ratings.end(), 0);
    return rating_sum / static_cast<int>(ratings.size());
}

SearchServer::QueryWord SearchServer::ParseQueryWord(std::string_view text) const {
    if (text.empty()) {
        throw std::invalid_argument("Query word is empty"s);
    }
    std::string_view word = text;
    bool is_minus = false;
    if (word[0] == '-') {
        is_minus = true;
        word = word.substr(1);
    }
    if (word.empty() || word[0] == '-' || !IsValidWord(word)) {
        throw std::invalid_argument("Query word "s + static_cast<std::string>(text) + " is invalid");
    }
    return {word, is_minus, IsStopWord(word)};
}

SearchServer::Query SearchServer::ParseQuery(std::string_view text, bool remove_duplicates) const {
    Query result;
    for (std::string_view word : SplitIntoWords(text)) {
        const auto query_word = ParseQueryWord(word);
        if (!query_word.is_stop) {
            if (query_word.is_minus) {
                result.minus_words.push_back(query_word.data);
            } else {
                result.plus_words.push_back(query_word.data);
            }
        }
    }
    if (remove_duplicates == true) {
        std::sort(result.plus_words.begin(), result.plus_words.end());
        auto last_plus_words = std::unique(result.plus_words.begin(), result.plus_words.end());
        result.plus_words.erase(last_plus_words, result.plus_words.end());

        std::sort(result.minus_words.begin(), result.minus_words.end());
        auto last_minus_words = std::unique(result.minus_words.begin(), result.minus_words.end());
        result.minus_words.erase(last_minus_words, result.minus_words.end());
    }
    return result;
}

double SearchServer::ComputeWordInverseDocumentFreq(std::string_view word) const {
    return std::log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
}