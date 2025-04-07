#pragma once
#include <string>

enum Type {

    TOK_LPAREN,
    TOK_RPAREN,
    TOK_WHITESPACE,
    TOK_EQ,
    TOK_DIGIT,
    TOK_INT,

    TOK_ID,
    TOK_VAL,
    TOK_VAR,
    TOK_ADD,
    TOK_IN,
    TOK_IF,
    TOK_THEN,
    TOK_ELSE,
    TOK_LET,
    TOK_FUN,
    TOK_CALL,

    TOK_NULL,
    TOK_EOF,
};

class Token {
protected:
    Type type_;
    std::string content;
    friend class Lexer;

public:
    Type type() const { return type_; }
    Token(const Type& type, const std::string& content = "")
        : type_(type), content(content) {}

    virtual explicit operator std::string() {
        std::string repr;
        switch (type_) {
            case (TOK_VAL):
                repr = "TOK_VAL";
                break;
            case (TOK_VAR):
                repr = "TOK_VAR";
                break;
            case (TOK_ADD):
                repr = "TOK_ADD";
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
            case (TOK_EOF):
                repr = "TOK_EOF";
                break;
            case (TOK_ID):
                repr = "TOK_ID";
                break;
            case (TOK_EQ):
                repr = "TOK_EQ";
                break;
            case (TOK_IN):
                repr = "TOK_IN";
                break;
            default:
                repr = "UNKNOWN";
        }
        return repr;
    }

    friend std::ostream& operator<<(std::ostream& os, const Token& t) {
        os << static_cast<std::string>(const_cast<Token&>(t));
        return os;
    }
};

class IntTok : public Token {
    int value_;

public:
    int value() const { return value_; }
    IntTok(int value) : Token(TOK_INT, std::to_string(value)), value_(value) {}

    explicit operator std::string() {
        return Token::operator std::string() + " " + content;
    }

    friend std::ostream& operator<<(std::ostream& os, const IntTok& t) {
        os << static_cast<std::string>(const_cast<IntTok&>(t));
        return os;
    }
};

class IdTok : public Token {
public:
    std::string id() const { return content; }
    IdTok(const std::string& id) : Token(TOK_ID, id) {}

    explicit operator std::string() {
        return Token::operator std::string() + " " + content;
    }

    friend std::ostream& operator<<(std::ostream& os, const IdTok& t) {
        os << static_cast<std::string>(const_cast<IdTok&>(t));
        return os;
    }
};
