#pragma once
#include "document.h"
#include "string_processing.h"
#include "concurrent_map.h"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include <deque>
#include <stdexcept>
#include <iterator>
#include <execution>
#include <thread>

using namespace std::string_literals;
using namespace std::literals;

class SearchServer {
public:
    template <typename StringContainer>
    explicit SearchServer(const StringContainer& stop_words);

    explicit SearchServer(std::string_view stop_words_text);

    explicit SearchServer(const std::string& stop_words_text);

    void AddDocument(int document_id, std::string_view document, DocumentStatus status, const std::vector<int>& ratings);

    template <typename DocumentPredicate>
    std::vector<Document> FindTopDocuments(std::string_view raw_query, DocumentPredicate document_predicate) const;
    template <typename ExecutionPolicy, typename DocumentPredicate>
    std::vector<Document> FindTopDocuments(const ExecutionPolicy& policy,
                                            std::string_view raw_query, DocumentPredicate document_predicate) const;

    std::vector<Document> FindTopDocuments(std::string_view raw_query, DocumentStatus status) const;
    std::vector<Document> FindTopDocuments(const std::execution::sequenced_policy&,
                                            std::string_view raw_query, DocumentStatus status) const;
    std::vector<Document> FindTopDocuments(const std::execution::parallel_policy&,
                                            std::string_view raw_query, DocumentStatus status) const;

    std::vector<Document> FindTopDocuments(std::string_view raw_query) const;
    std::vector<Document> FindTopDocuments(const std::execution::sequenced_policy&,
                                           std::string_view raw_query) const;
    std::vector<Document> FindTopDocuments(const std::execution::parallel_policy&,
                                            std::string_view raw_query) const;

    int GetDocumentCount() const;

    std::set<int>::const_iterator begin() const;
    std::set<int>::const_iterator end() const;

    const std::map<std::string_view, double>& GetWordFrequencies(int document_id) const;

    void RemoveDocument(int document_id);
    void RemoveDocument(const std::execution::sequenced_policy&, int document_id);
    void RemoveDocument(const std::execution::parallel_policy&, int document_id);

    std::tuple<std::vector<std::string_view>, DocumentStatus> MatchDocument(std::string_view raw_query, int document_id) const;

    std::tuple<std::vector<std::string_view>, DocumentStatus> MatchDocument(const std::execution::sequenced_policy&,
                                                                            std::string_view raw_query, int document_id) const;
    std::tuple<std::vector<std::string_view>, DocumentStatus> MatchDocument(const std::execution::parallel_policy&,
                                                                            std::string_view raw_query, int document_id)  const;

private:
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };
    const std::set<std::string, std::less<>> stop_words_;
    std::deque<std::string> buffer_;
    std::map<std::string_view, std::map<int, double>> word_to_document_freqs_;
    std::map<int, std::map<std::string_view, double>> freqs_of_document_words_;
    std::map<int, DocumentData> documents_;
    std::set<int> document_ids_;

    bool IsStopWord(std::string_view word) const;

    static bool IsValidWord(std::string_view word);

    std::vector<std::string_view> SplitIntoWordsNoStop(std::string_view text) const;

    static int ComputeAverageRating(const std::vector<int>& ratings);

    struct QueryWord {
        std::string_view data;
        bool is_minus;
        bool is_stop;
    };

    QueryWord ParseQueryWord(std::string_view text) const;

    struct Query {
        std::vector<std::string_view> plus_words;
        std::vector<std::string_view> minus_words;
    };

    Query ParseQuery(std::string_view text, bool remove_duplicates = false) const;

    double ComputeWordInverseDocumentFreq(std::string_view word) const;

    template <typename DocumentPredicate>
    std::vector<Document> FindAllDocuments(const Query& query, DocumentPredicate document_predicate) const;
    template <typename DocumentPredicate>
    std::vector<Document> FindAllDocuments(const std::execution::sequenced_policy&,
                                            const Query& query, DocumentPredicate document_predicate) const;
    template <typename DocumentPredicate>
    std::vector<Document> FindAllDocuments(const std::execution::parallel_policy&,
                                            const Query& query, DocumentPredicate document_predicate) const;
};

template <typename StringContainer>
SearchServer::SearchServer(const StringContainer& stop_words)
    : stop_words_(MakeUniqueNonEmptyStrings(stop_words))
{
    if (!std::all_of(stop_words_.begin(), stop_words_.end(), IsValidWord)) {
        throw std::invalid_argument("Some of stop words are invalid");
    }
}

template <typename DocumentPredicate>
std::vector<Document> SearchServer::FindTopDocuments(std::string_view raw_query,
                                                     DocumentPredicate document_predicate) const {
    return SearchServer::FindTopDocuments(std::execution::seq, raw_query, document_predicate);
}

template <typename ExecutionPolicy, typename DocumentPredicate>
std::vector<Document> SearchServer::FindTopDocuments(const ExecutionPolicy& policy,
                                                     std::string_view raw_query, DocumentPredicate document_predicate) const {
    const auto query = ParseQuery(raw_query, true);
    auto matched_documents = FindAllDocuments(policy, query, document_predicate);

    const double epsilon = 1e-6;
    std::sort(
             policy,
             matched_documents.begin(), matched_documents.end(),
             [epsilon](const Document& lhs, const Document& rhs) {
                 return lhs.relevance > rhs.relevance
                     || (std::abs(lhs.relevance - rhs.relevance) < epsilon && lhs.rating > rhs.rating);
             });
    const int max_result_document_count = 5;
    if (matched_documents.size() > max_result_document_count) {
        matched_documents.resize(max_result_document_count);
    }
    return matched_documents;
}

template <typename DocumentPredicate>
std::vector<Document> SearchServer::FindAllDocuments(const Query& query,
                                      DocumentPredicate document_predicate) const {
    return SearchServer::FindAllDocuments(std::execution::seq, query, document_predicate);
}

template <typename DocumentPredicate>
std::vector<Document> SearchServer::FindAllDocuments(const std::execution::sequenced_policy&,
                                                     const Query& query, DocumentPredicate document_predicate) const {
    std::map<int, double> document_to_relevance;
    for (const std::string_view& word : query.plus_words) {
        if (word_to_document_freqs_.count(word) == 0) {
            continue;
        }
        const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
        for (const auto& [document_id, term_freq] : word_to_document_freqs_.at(word)) {
            const auto& document_data = documents_.at(document_id);
            if (document_predicate(document_id, document_data.status, document_data.rating)) {
                document_to_relevance[document_id] += term_freq * inverse_document_freq;
            }
        }
    }
    for (const std::string_view& word : query.minus_words) {
        if (word_to_document_freqs_.count(word) == 0) {
            continue;
        }
        for (const auto& [document_id, _] : word_to_document_freqs_.at(word)) {
            document_to_relevance.erase(document_id);
        }
    }

    std::vector<Document> matched_documents;
    for (const auto& [document_id, relevance] : document_to_relevance) {
        matched_documents.push_back(
            {document_id, relevance, documents_.at(document_id).rating});
    }
    return matched_documents;
}

template <typename DocumentPredicate>
std::vector<Document> SearchServer::FindAllDocuments(const std::execution::parallel_policy&,
                                                     const Query& query, DocumentPredicate document_predicate) const {
    ConcurrentMap<int, double> document_to_relevance(std::thread::hardware_concurrency());
    std::for_each(
                  std::execution::par,
                  query.plus_words.begin(), query.plus_words.end(),
                  [&](std::string_view word) {
                    if (word_to_document_freqs_.count(word) != 0) {
                        const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
                        for (const auto& [document_id, term_freq] : word_to_document_freqs_.at(word)) {
                            const auto& document_data = documents_.at(document_id);
                            if (document_predicate(document_id, document_data.status, document_data.rating)) {
                                document_to_relevance[document_id].ref_to_value += term_freq * inverse_document_freq;
                            }
                        }
                    }
                });
    std::for_each(
                  std::execution::par,
                  query.minus_words.begin(), query.minus_words.end(),
                  [&](std::string_view word) {
                    if (word_to_document_freqs_.count(word) != 0) {
                        for (const auto& [document_id, _] : word_to_document_freqs_.at(word)) {
                            document_to_relevance.BuildOrdinaryMap().erase(document_id);
                        }
                    }
                });

    std::vector<Document> matched_documents;
    for (const auto& [document_id, relevance] : document_to_relevance.BuildOrdinaryMap()) {
        matched_documents.push_back(
            {document_id, relevance, documents_.at(document_id).rating});
    }
    return matched_documents;
}