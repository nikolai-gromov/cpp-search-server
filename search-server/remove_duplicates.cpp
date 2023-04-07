#include "remove_duplicates.h"

void RemoveDuplicates(SearchServer& search_server) {
    std::map<std::set<std::string>, int> document_words_id;
    std::set<int> remove_id;
    for (auto it = search_server.begin(); it != search_server.end(); ++it) {
        const std::map<std::string, double> words_freqs = search_server.GetWordFrequencies(*it);
        std::set<std::string> document_words;
        for (auto [word, freq] : words_freqs) {
            document_words.insert(word);
        }

        auto it2 = document_words_id.find(document_words);
        if (it2 != document_words_id.end()) {
            if (document_words_id.at(document_words) > *it) {
                remove_id.insert(document_words_id.at(document_words));
                document_words_id.erase(it2);
                document_words_id.insert({document_words, *it});
            } else {
                remove_id.insert(*it);
            }
        } else {
            document_words_id.insert({document_words, *it});
        }
    }
    for (auto id : remove_id) {
        std::cout << "Found duplicate document id "s << id << std::endl;
        search_server.RemoveDocument(id);
    }
}