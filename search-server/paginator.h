#pragma once
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

using namespace std::string_literals;

std::ostream& operator<<(std::ostream& out, const Document& document);

void PrintDocument(const Document& document);

void PrintMatchDocumentResult(int document_id, const std::vector<std::string>& words, DocumentStatus status);

template <typename Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end);

    Iterator begin() const;
    Iterator end() const;
    std::size_t size() const;
private:
    Iterator first_, last_;
    std::size_t size_;
};

template <typename Iterator>
std::ostream& operator<<(std::ostream& out, const IteratorRange<Iterator>& range);

template <typename Iterator>
class Paginator {
public:
    Paginator(Iterator begin, Iterator end, size_t page_size);

    auto begin() const;
    auto end() const;
    std::size_t size() const;
private:
    std::vector<IteratorRange<Iterator>> pages_;
};

template <typename Container>
auto Paginate(const Container& c, std::size_t page_size);

std::ostream& operator<<(std::ostream& out, const Document& document) {
    out << "{ "s
        << "document_id = "s << document.id << ", "s
        << "relevance = "s << document.relevance << ", "s
        << "rating = "s << document.rating << " }"s;
    return out;
}

void PrintDocument(const Document& document) {
    std::cout << "{ "s
         << "document_id = "s << document.id << ", "s
         << "relevance = "s << document.relevance << ", "s
         << "rating = "s << document.rating << " }"s << std::endl;
}

void PrintMatchDocumentResult(int document_id, const std::vector<std::string>& words, DocumentStatus status) {
    std::cout << "{ "s
         << "document_id = "s << document_id << ", "s
         << "status = "s << static_cast<int>(status) << ", "s
         << "words ="s;
    for (const std::string& word : words) {
        std::cout << ' ' << word;
    }
    std::cout << "}"s << std::endl;
}


template <typename Iterator>
IteratorRange<Iterator>::IteratorRange(Iterator begin, Iterator end)
    : first_(begin)
    , last_(end)
    , size_(std::distance(first_, last_)) {
}

template <typename Iterator>
Iterator IteratorRange<Iterator>::begin() const {
    return first_;
}
template <typename Iterator>
Iterator IteratorRange<Iterator>::end() const {
    return last_;
}
template <typename Iterator>
std::size_t IteratorRange<Iterator>::size() const {
    return size_;
}

template <typename Iterator>
std::ostream& operator<<(std::ostream& out, const IteratorRange<Iterator>& range) {
    for (Iterator it = range.begin(); it != range.end(); ++it) {
        out << *it;
    }
    return out;
}

template <typename Iterator>
Paginator<Iterator>::Paginator(Iterator begin, Iterator end, size_t page_size) {
    for (size_t left = std::distance(begin, end); left > 0;) {
        const size_t current_page_size = std::min(page_size, left);
        const Iterator current_page_end = std::next(begin, current_page_size);
        pages_.push_back({begin, current_page_end});
        left -= current_page_size;
        begin = current_page_end;
    }
}

template <typename Iterator>
auto Paginator<Iterator>::begin() const {
    return pages_.begin();
}
template <typename Iterator>
auto Paginator<Iterator>::end() const {
    return pages_.end();
}
template <typename Iterator>
std::size_t Paginator<Iterator>::size() const {
    return pages_.size();
}

template <typename Container>
auto Paginate(const Container& c, std::size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}