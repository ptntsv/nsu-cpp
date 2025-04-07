#ifndef IO_FILES_H
#define IO_FILES_H
#include <fstream>
#include <iostream>

#include "io_reader_writer.h"

class io_files : public io_reader_writer {
public:
    io_files(OPENMODE omode) : io_base(omode) {}
    void open(const std::string& filename) {
        std::string flags{};
        if (openmode() & app)
            flags += "a";
        else if (openmode() & out)
            flags += "r";
        if (openmode() & in)
            flags += "+";
        file = std::fopen(filename.c_str(), flags.c_str());
        if (!file) {
            std::cerr << "Error opening file " << filename << std::endl;
            iostate_ = badbit;
            return;
        }
        is_open_ = true;
    }
    void close() {
        std::fclose(file);
        is_open_ = false;
    }

    io_files& operator<<(int value) override {
        std::fwrite(std::to_string(value).c_str(), sizeof(int), 1, file);
        return *this;
    }
    io_files& operator<<(double value) override {
        std::fwrite(std::to_string(value).c_str(), sizeof(int), 1, file);
        return *this;
    }
    io_files& operator<<(char value) override {
        std::fwrite(std::to_string(value).c_str(), sizeof(int), 1, file);
        return *this;
    }
    io_files& operator<<(const std::string& value) override {
        std::fwrite(value.c_str(), sizeof(int), 1, file);
        return *this;
    }

    void skip_whitespaces() const {
        int ch;
        while ((ch = getc(file)) != EOF && std::isspace(ch)) {
        }
        ungetc(ch, file);
    }

    int read_int() {
        skip_whitespaces();
        std::string acc{};
        int ch;
        while ((ch = getc(file)) != EOF && (std::isdigit(ch) || ch == '-')) {
            if (ch == EOF)
                iostate_ = eofbit;
            acc += ch;
        }
        try {
            return std::stoi(acc);
        } catch (const std::invalid_argument& e) {
            iostate_ = failbit;
            return 0;
        }
    }
    double read_double() {
        skip_whitespaces();
        std::string acc{};
        int ch;
        while ((ch = getc(file)) != EOF &&
               (std::isdigit(ch) || ch == '-' || ch == '.')) {
            if (ch == EOF)
                iostate_ = eofbit;
            acc += ch;
        }
        try {
            return std::stod(acc);
        } catch (const std::invalid_argument& e) {
            iostate_ = failbit;
            return 0;
        }
    }

    std::string read_string() {
        skip_whitespaces();
        std::string acc{};
        int ch;
        while ((ch = getc(file)) != EOF && !std::isspace(ch)) {
            if (ch == EOF)
                iostate_ = eofbit;
            acc += ch;
        }
        return acc;
    }
    io_files& operator>>(int& out) override {
        out = read_int();
        return *this;
    }
    io_files& operator>>(char& out) override {
        int ch = getc(file);
        if (ch == EOF)
            iostate_ = eofbit;
        out = ch;
        return *this;
    }
    io_files& operator>>(double& out) override {
        out = read_double();
        return *this;
    }
    io_files& operator>>(std::string& out) override {
        out = read_string();
        return *this;
    }

private:
    FILE* file = nullptr;
};

#endif  // IO_FILES_H
