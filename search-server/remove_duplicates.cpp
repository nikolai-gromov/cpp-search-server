#include "remove_duplicates.h"

void RemoveDuplicates(SearchServer& search_server) {
    std::map<std::set<std::string>, int> document_words_id;
    std::set<int> remove_id;
    for (const int document_id : search_server) {
        const std::map<std::string, double> words_freqs = search_server.GetWordFrequencies(document_id);
        std::set<std::string> document_words;
        for (auto [word, freq] : words_freqs) {
            document_words.insert(word);
        }

        auto it = document_words_id.find(document_words);
        if (it != document_words_id.end()) {
            remove_id.insert(document_id);
        } else {
            document_words_id.insert({document_words, document_id});
        }
    }
    for (auto id : remove_id) {
        std::cout << "Found duplicate document id "s << id << std::endl;
        search_server.RemoveDocument(id);
    }
}