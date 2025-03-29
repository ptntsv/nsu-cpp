#pragma once
#include <stdexcept>

class Expression;

class Env {
    std::map<std::string, Expression*> lookupTable =
        std::map<std::string, Expression*>();
};

class Expression {
public:
    Env* env;
    virtual Expression* eval() = 0;
    virtual int getVal() { throw std::runtime_error("not val expression"); }
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
    int value;
    Expression* eval() { return this; }
    int getVal() { return value; }

public:
    Val(int value) : value(value) {}
};

class Add : public Binary {
public:
    Expression* eval() {
        return new Val(left->eval()->getVal() + right->eval()->getVal());
    }
    Add(Expression* left, Expression* right) : Binary(left, right) {};
};

class Var : public Expression {
    const std::string id;
    Var(const std::string& id) : id(id) {}
    Expression* eval() {}
};
