#pragma once
#include <string>
#include <vector>
#include <fstream>

#define CSV_READER_BUFFER_LENGTH (4 * 1024)

class reader_csv
{
public:

    class iterator {
    public:
        explicit iterator(reader_csv* p, bool end = false) : _reader(p) {
            if (!end) {
                entry_row.reserve(50);
                _current_row = 0;
                next();
            }
        }

        iterator& operator++() {
            next();
            return *this;
        }

        iterator operator++(int) {
            iterator i = (*this);
            ++(*this);
            return i;
        }

        bool operator==(const iterator& other) const {
                return _current_row == other._current_row
                && entry_row.size() == other.entry_row.size();
        }
        bool operator!=(const iterator& other) const { return !(*this == other); }
        const std::vector<std::string>& operator*() const { return entry_row; }
        const std::vector<std::string>* operator->() const { return &entry_row; }

    private:
        std::vector<std::string> entry_row{};
        reader_csv* _reader;
        int _current_row = -1;

        void next();
    };

    reader_csv(std::string location, bool header = 1);
    reader_csv(std::string location, std::vector<std::string> expected_headers);
    ~reader_csv() {};
    std::vector<std::string> read_line();
    void validate_header() {};
    iterator begin() { return iterator(this); };
    iterator end() { return iterator(this, true); };

private:
    std::ifstream _ifstream;
    std::streamsize _file_length = 0;
    unsigned short _buffer_position = 0;
    std::streamsize _buffer_length = 0;
    char _buffer[CSV_READER_BUFFER_LENGTH] = { 0 };

    void find_start();
};