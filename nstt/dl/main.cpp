#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "src/expressions/env.hpp"
#include "src/expressions/expr.hpp"
#include "src/lexer.hpp"
#include "src/token.hpp"

using std::cout;
using std::endl;

std::string readFromFile(const std::string& fileName) {
    std::string content;
    std::string tmp;
    std::ifstream file{fileName};
    while (std::getline(file, tmp)) {
        content += tmp + '\n';
    }
    return content;
}

class Parser {
    size_t cursor = 0, start = 0, back = 0;
    std::vector<Token*> tokens;

    bool is_end() { return cursor >= tokens.size(); }
    Token* advance() { return tokens.at(cursor++); }
    Token* next() { return tokens.at(cursor); }

    void error(const std::string& expected, Token* got) {
        throw std::runtime_error{
            "expected: " + expected +
            ", got: " + (got ? static_cast<std::string>(*got) : "nullptr")};
    }

public:
    Expression* val() {
        Token* t = advance();
        IntTok* intgr = dynamic_cast<IntTok*>(t);
        if (!intgr)
            error("TOK_INT", t);
        return intgr ? new Val(intgr) : nullptr;
    }
    Expression* var() {
        Token* t = advance();
        IdTok* id = dynamic_cast<IdTok*>(t);
        if (!id)
            error("TOK_ID", t);
        return id ? new Var(id) : nullptr;
    }
    Expression* add() { return new Add(expr(), expr()); }

    void match(const Type& exp, const Type& actual) {
        if (exp == actual)
            advance();
        else
         ;
    }

    Expression* expr() {
        Expression* out;
        if (next()->type() == TOK_LPAREN) {
            advance();
            switch (advance()->type()) {
                case TOK_VAL:
                    out = val();
                    break;
                case TOK_VAR:
                    out = var();
                    break;
                case TOK_ADD:
                    out = add();
                    break;
                default:
                    return nullptr;
            }
        } else {
            error("TOK_LPAREN", next());
        }
        if (next()->type() == TOK_RPAREN)
            advance();
        return out;
    }
    Parser(const std::vector<Token*>& tokens) : tokens(tokens) {}
};

int main() {
    Env env;
    std::string content = readFromFile("../tests/inputs/t1.txt");
    Lexer l{content};
    std::vector<Token*> ts = l.lex();
    Parser p{ts};
    Expression* tree{p.expr()};
    Expression* evaled = tree->eval();
    std::cout << evaled->get_value() << endl;

    for (auto& t : ts) {
        delete t;
    }
    delete tree;
    delete evaled;
    return 0;
}
