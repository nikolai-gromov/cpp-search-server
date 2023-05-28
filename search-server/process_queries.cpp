#include "process_queries.h"
#include <execution>

std::vector<std::vector<Document>> ProcessQueries(
    const SearchServer& search_server,
    const std::vector<std::string>& queries) {
    std::vector<std::vector<Document>> documents(queries.size());
    std::transform(std::execution::par,
                    queries.begin(), queries.end(),
                    documents.begin(),
                    [&search_server](const std::string& query) {
                        return search_server.FindTopDocuments(query);
    });
    return documents;
}

std::vector<Document> ProcessQueriesJoined(
    const SearchServer& search_server,
    const std::vector<std::string>& queries) {
    std::vector<Document> result;
    result.reserve(queries.size());
    auto documents = ProcessQueries(search_server, queries);
    for (size_t i = 0; i != documents.size(); ++i) {
        for (const Document& document : documents[i]) {
            result.push_back(document);
        }
    }
    return result;
}