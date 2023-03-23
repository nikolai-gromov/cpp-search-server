#pragma once
#include <stdexcept>
#include <iostream>
#include <vector>
#include <string>

#include "search_server.h"
#include "paginator.h"

using namespace std::string_literals;

void AddDocument(SearchServer& search_server, int document_id, const std::string& document,
                 DocumentStatus status, const std::vector<int>& ratings);

void FindTopDocuments(const SearchServer& search_server, const std::string& raw_query);

void MatchDocuments(const SearchServer& search_server, const std::string& query);