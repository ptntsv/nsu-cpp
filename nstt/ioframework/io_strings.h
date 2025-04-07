#ifndef IO_STRINGS_H
#define IO_STRINGS_H
#include <iostream>
#include <ostream>

#include "io_reader_writer.h"

class io_strings : public io_reader_writer {
    std::string content_;
    size_t pos_ = 0;

    bool is_end() const { return pos_ == content_.size(); }
    char cur() const { return !is_end() ? content_[pos_] : 0; }
    void inc() {
        ++pos_;
        if (is_end()) {
            iostate_ = eofbit;
        }
    }

    void skip_whitespaces() {
        while (std::isspace(cur())) inc();
    }

    int read_int() {
        skip_whitespaces();
        const size_t start = pos_;
        while ((cur() == '-' || std::isdigit(cur())) && !is_end()) inc();
        try {
            return std::stoi(content_.substr(start, pos_ - start));
        } catch (const std::invalid_argument& e) {
            iostate_ = failbit;
            return 0;
        }
    }
    double read_double() {
        skip_whitespaces();
        const size_t start = pos_;
        while ((cur() == '-' || cur() == '.' || std::isdigit(cur())) &&
               !is_end())
            inc();
        try {
            return std::stod(content_.substr(start, pos_ - start));
        } catch (const std::invalid_argument& e) {
            iostate_ = failbit;
            return 0;
        }
    }
    char read_char() {
        const char c = cur();
        inc();
        return c;
    }

    std::string read_string() {
        skip_whitespaces();
        const size_t start = pos_;
        while (!is_end() && !std::isspace(cur())) inc();
        return content_.substr(start, pos_ - start);
    }

public:
    io_strings(const std::string& content = "") : content_(content) {}

    std::string content() const { return content_; }

    io_strings& operator<<(int value) override {
        content_ += std::to_string(value);
        return *this;
    }
    io_strings& operator<<(double value) override {
        content_ += std::to_string(value);
        return *this;
    }
    io_strings& operator<<(char value) override {
        content_ += std::to_string(value);
        return *this;
    }
    io_strings& operator<<(const std::string& value) override {
        content_ += value;
        return *this;
    }

    io_strings& operator>>(int& out) override {
        out = read_int();
        return *this;
    }
    io_strings& operator>>(char& out) override {
        out = read_char();
        return *this;
    }
    io_strings& operator>>(double& out) override {
        out = read_double();
        return *this;
    }
    io_strings& operator>>(std::string& out) override {
        out = read_string();
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os,
                                    const io_strings& sbuffer) {
        os << sbuffer.content_ << std::endl;
        return os;
    }
};

#endif  // IO_STRINGS_H
