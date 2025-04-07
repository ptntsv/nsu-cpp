#pragma once
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#include "../token.hpp"

class Expression;

class Env {
    std::map<std::string, Expression*> lookupTable =
        std::map<std::string, Expression*>();
};

class Expression {
public:
    Env* env;
    virtual Expression* eval() = 0;
    virtual int get_value() { throw std::runtime_error("not val expression"); }
    virtual void print() { std::cout << static_cast<std::string>(*this); }
    virtual operator std::string() { return ""; };
    virtual ~Expression() {};
};

class Binary : public Expression {
protected:
    Expression* left;
    Expression* right;

public:
    Binary(Expression* left, Expression* right) : left(left), right(right) {}
    ~Binary() {
        delete left;
        delete right;
    }
};

class Unary : public Expression {
    Expression* expr;
    Unary(Expression* expr) : expr(expr) {}
};

class Val : public Expression {
    int value_;
    Expression* eval() { return this; }
    int get_value() { return value_; }

public:
    Val(int value) : value_(value) {}
    Val(IntTok* token) : value_(token->value()) {}
    operator std::string() { return "(val " + std::to_string(value_) + ")"; }
};

class Add : public Binary {
public:
    Expression* eval() {
        return new Val(left->eval()->get_value() + right->eval()->get_value());
    }
    Add(Expression* left, Expression* right) : Binary(left, right) {};
    operator std::string() const {
        std::string lstr = *left;
        std::string rstr = *right;
        return "(add " + lstr + " " + rstr + ")";
    }
};

class Var : public Expression {
    const std::string id;

public:
    Var(const std::string& id) : id(id) {}
    Var(IdTok* token) : id(token->id()) {}
    Expression* eval() {}
    operator std::string() { return "(var " + id + ")"; }
};
