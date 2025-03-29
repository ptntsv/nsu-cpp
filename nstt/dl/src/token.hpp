#pragma once
#include <string>

enum Type {
    TOK_VAL,
    TOK_VAR,
    TOK_ADD,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_WHITESPACE,
    TOK_DIGIT,
    TOK_INT,
    TOK_NULL,
};
class TokenType {
    Type type;
    friend std::ostream& operator<<(std::ostream& os, TokenType& t) {
        os << static_cast<std::string>(t);
        return os;
    }

public:
    explicit operator std::string() {
        std::string repr;
        switch (type) {
            case (TOK_VAL):
                repr = "TOK_VAL";
                break;
            case (TOK_VAR):
                repr = "TOK_VAR";
                break;
            case (TOK_LPAREN):
                repr = "TOK_LPAREN";
                break;
            case (TOK_RPAREN):
                repr = "TOK_RPAREN";
                break;
            case (TOK_WHITESPACE):
                repr = "TOK_WHITESPACE";
                break;
            case (TOK_DIGIT):
                repr = "TOK_DIGIT";
                break;
            case (TOK_NULL):
                repr = "TOK_NULL";
                break;
            case (TOK_INT):
                repr = "TOK_INT";
                break;
            default:
                repr = "UNKNOWN";
        }
        return repr;
    }
    TokenType(const Type type) : type(type) {}
    friend class Token;
};

class Token {
protected:
    std::string content;
    TokenType type;
    friend class Lexer;

public:
    Type getType() const { return type.type; }
    Token(TokenType type, const std::string& content = "")
        : type(type), content(content) {}

    virtual explicit operator std::string() {
        return static_cast<std::string>(type);
    }

    friend std::ostream& operator<<(std::ostream& os, const Token& t) {
        os << static_cast<std::string>(const_cast<Token&>(t));
        return os;
    }
};

class IntTok : public Token {
    int value;

public:
    IntTok(int value) : Token(TOK_INT, std::to_string(value)), value(value) {}

    explicit operator std::string() {
        return static_cast<std::string>(type) + " " + content;
    }

    friend std::ostream& operator<<(std::ostream& os, const IntTok& t) {
        os << static_cast<std::string>(const_cast<IntTok&>(t));
        return os;
    }
};
