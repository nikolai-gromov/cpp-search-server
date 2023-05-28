#pragma once
#include <deque>
#include <vector>
#include <string>
#include <string_view>
#include "search_server.h"

class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server);

    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(std::string_view raw_query, DocumentPredicate document_predicate);

    std::vector<Document> AddFindRequest(std::string_view raw_query, DocumentStatus status);

    std::vector<Document> AddFindRequest(std::string_view raw_query);

    int GetNoResultRequests() const;

private:
    struct QueryResult {
        std::uint64_t timestamp;
        int results;
    };
    std::deque<QueryResult> requests_;
    const SearchServer& search_server_;
    int no_results_requests_;
    std::uint64_t current_time_;
    const static int min_in_day_ = 1440;

    void AddRequest(int results_num);
};