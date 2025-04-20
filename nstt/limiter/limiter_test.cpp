#include "limiter.h"

#include "gtest/gtest.h"

class Foo : Limiter<Foo, 5> {
public:
    Foo() { std::cout << "Foo" << std::endl; }
};

TEST(LimitSuite, Inits) {
    Foo foos[6];
}
