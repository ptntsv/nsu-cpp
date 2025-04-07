#pragma once
#include <cstdio>
#include <map>
#include <string>
#include <vector>

#include "token.hpp"

class Lexer {
private:
    size_t cursor = 0;
    size_t start = 0;
    size_t back = 0;
    std::string buff = "";

    std::map<std::string, Type> keywords = {
        {"val", TOK_VAL}, {"var", TOK_VAR},      {"add", TOK_ADD},
        {"if", TOK_IF},   {"then", TOK_THEN},    {"else", TOK_ELSE},
        {"let", TOK_LET}, {"function", TOK_FUN}, {"call", TOK_CALL}};

    bool isEnd() { return cursor >= buff.length(); }

    char advance() { return buff[cursor++]; }

    char peek() { return isEnd() ? '\0' : buff.at(cursor); }

    char peekNext() {
        return cursor + 1 >= buff.length() ? '\0' : buff.at(cursor + 1);
    }

    char peekPrev() { return cursor == 0 ? '\0' : buff.at(cursor - 1); }

    bool isKeyword(std::string key) {
        return keywords.find(key) != keywords.end();
    }

    void readIdentifier() {
        while (isalpha(peek()) || isdigit(peek()) || peek() == '_') advance();
    }

    void readNumber() {
        while (isdigit(peek())) advance();
        if (peek() == '.') {
            if (!isdigit(peekNext()))
                while (isdigit(peek())) advance();
        }
    }

    void skipWhitespace() {
        for (;;) {
            char c = peek();
            switch (c) {
                case ' ':
                case '\r':
                case '\t':
                    advance();
                    break;
                case '\n':
                    advance();
                    break;
                default:
                    return;
            }
        }
    }

    Token* scanToken() {
        skipWhitespace();
        start = cursor;
        if (isEnd())
            return new Token(TOK_EOF, "eof");
        char ch = advance();
        switch (ch) {
            case '(':
                return new Token(TOK_LPAREN, "(");
            case ')':
                return new Token(TOK_RPAREN, ")");
            default:
                if (isalpha(ch)) {
                    readIdentifier();
                    std::string substring = buff.substr(start, cursor - start);
                    if (!isKeyword(substring)) {
                        return new IdTok(substring);
                    } else {
                        return new Token(keywords[substring], substring);
                    }
                } else if (ch == '-' || isdigit(ch)) {
                    readNumber();
                    std::string substring = buff.substr(start, cursor - start);
                    return new IntTok(std::stoi(substring));
                }
        }
        return new Token(TOK_NULL, "null");
    }

    bool matchNextToken(const Type& type) {
        back = cursor;
        if (scanToken()->type() == type)
            return true;
        cursor = back;
        return false;
    }

public:
    std::vector<Token*> lex() {
        std::vector<Token*> tokens;
        for (;;) {
            Token* token = scanToken();
            tokens.push_back(token);
            if (token->type() == TOK_EOF)
                break;
        }
        return tokens;
    }

    Lexer(std::string buff) : buff(buff) {}

    Lexer() = default;
};
