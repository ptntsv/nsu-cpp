#include <iostream>
#include <map>
#include <tuple>

#include "../src/treap/treap.hpp"
#include "gtest/gtest.h"

using namespace std;

struct Foo {
    int hidden_key;
    Foo() = default;

    Foo(int key)
        : hidden_key(key) {
    }

    bool operator==(const Foo& other) const {
        return hidden_key == other.hidden_key;
    }

    bool operator<(const Foo& other) const {
        return hidden_key < other.hidden_key;
    }

    bool operator>(const Foo& other) const {
        return hidden_key > other.hidden_key;
    }
};

map<int, int> int_init = {
    {8, 10}, {12, 8}, {14, 14}, {15, 4}, {18, 9}, {23, 6}, {24, 15}, {25, 11},
};

map<Foo, int> foo_init = {
    {Foo(8), 10}, {Foo(12), 8}, {Foo(14), 14}, {Foo(15), 4},
    {Foo(18), 9}, {Foo(23), 6}, {Foo(24), 15}, {Foo(25), 11},
};

template <typename T>
map<T, int> init_map_factory() {
    throw std::runtime_error("not implemented");
}

template <>
map<int, int> init_map_factory() {
    return int_init;
}

template <>
map<Foo, int> init_map_factory() {
    return foo_init;
}

template <typename T>
class TreapTest : public ::testing::Test {
protected:
    Treap<T>* t;
    map<T, int> init_map;

    TreapTest()
        : init_map(init_map_factory<T>()) { t = new Treap<T>{init_map}; }

    ~TreapTest() { delete t; }
};

TYPED_TEST_SUITE_P(TreapTest);

TYPED_TEST_P(TreapTest, TreapBuilds) {
    ASSERT_EQ(this->t->isValid(), true);
    delete this->t->root();
}


TYPED_TEST_P(TreapTest, TreapSplits) {
    std::pair<Treap<TypeParam>*, Treap<TypeParam>*> two = this->t->split(20);

    ASSERT_EQ(two.first->isValid(), true);
    ASSERT_EQ(two.second->isValid(), true);

    delete two.first->root();
    delete two.second->root();

    delete two.first;
    delete two.second;
}


TYPED_TEST_P(TreapTest, TreapSplitsWithEmptyLeft) {
    std::pair<Treap<TypeParam>*, Treap<TypeParam>*> two = this->t->split(5);

    ASSERT_EQ(two.first->isValid(), true);
    ASSERT_EQ(two.second->isValid(), true);
    ASSERT_EQ(two.first->empty(), true);
    ASSERT_EQ(two.second->empty(), false);

    delete two.second->root();

    delete two.first;
    delete two.second;
}

TYPED_TEST_P(TreapTest, TreapSplitsWithEmptyRight) {
    std::pair<Treap<TypeParam>*, Treap<TypeParam>*> two = this->t->split(30);

    ASSERT_EQ(two.first->isValid(), true);
    ASSERT_EQ(two.second->isValid(), true);
    ASSERT_EQ(two.first->empty(), false);
    ASSERT_EQ(two.second->empty(), true);

    delete two.first->root();
    delete two.second->root();

    delete two.first;
    delete two.second;
}

TYPED_TEST_P(TreapTest, TreapMerges20) {
    std::pair<Treap<TypeParam>*, Treap<TypeParam>*> two = this->t->split(20);
    two.first->merge(two.second);

    ASSERT_EQ(two.first->isValid(), true);

    delete two.first->root();
    delete two.first;
    delete two.second;
}

TYPED_TEST_P(TreapTest, TreapMerges16) {
    std::pair<Treap<TypeParam>*, Treap<TypeParam>*> two = this->t->split(16);
    two.first->merge(two.second);

    ASSERT_EQ(two.first->isValid(), true);

    delete two.first->root();
    delete two.first;
    delete two.second;
}

TYPED_TEST_P(TreapTest, TreapMergesWithEmptyR) {
    std::pair<Treap<TypeParam>*, Treap<TypeParam>*> two = this->t->split(300);
    two.first->merge(two.second);

    ASSERT_EQ(two.first->isValid(), true);

    delete two.first->root();
    delete two.first;
    delete two.second;
}

TYPED_TEST_P(TreapTest, TreapMergesWithEmptyL) {
    std::pair<Treap<TypeParam>*, Treap<TypeParam>*> two = this->t->split(2);
    two.first->merge(two.second);

    ASSERT_EQ(two.first->isValid(), true);

    delete two.first->root();
    delete two.first;
    delete two.second;
}

TYPED_TEST_P(TreapTest, TreapInserts) {
    this->t->insert(7, 20);
    ASSERT_EQ(this->t->isValid(), true);
    ASSERT_EQ(this->t->find(20)->priority_, 7);

    this->t->insert(5, 99);
    ASSERT_EQ(this->t->isValid(), true);
    ASSERT_EQ(this->t->find(99)->priority_, 5);
    this->t->insert(42, 21);
    ASSERT_EQ(this->t->isValid(), true);
    ASSERT_EQ(this->t->find(21)->priority_, 42);
    delete this->t->root();
}

TYPED_TEST_P(TreapTest, TreapRemoves) {
    this->t->remove(23);
    delete this->t->root();
}

TYPED_TEST_P(TreapTest, TreapRemovesNull) {
    this->t->remove(228);
    delete this->t->root();
}

TYPED_TEST_P(TreapTest, TreapRemovesRightmost) {
    this->t->remove(24);

    ASSERT_EQ(this->t->isValid(), true);

    delete this->t->root();
}

TYPED_TEST_P(TreapTest, TreapRemovesAll) {
    for (typename map<TypeParam, int>::const_iterator it = this->init_map.begin();
         it != this->init_map.end(); it++) {
        this->t->remove(it->first);
    }

    ASSERT_EQ(this->t->empty(), true);

    delete this->t->root();
}

TYPED_TEST_P(TreapTest, TreapCopyConstructor) {
    Treap<TypeParam> twin{*this->t};
    ASSERT_TRUE(twin.isValid());
    ASSERT_TRUE(twin == *this->t);
    twin.insert(69, 96);
    ASSERT_FALSE(twin == *this->t);
    delete this->t->root();
    delete twin.root();
}

TYPED_TEST_P(TreapTest, TreapAssigns) {
    Treap<TypeParam> twin{*this->t};
    Treap<TypeParam> twin1{5, 2};
    ASSERT_TRUE(twin == *this->t);
    twin = twin1;
    ASSERT_TRUE(twin == twin1);
    ASSERT_FALSE(twin == *this->t);
    delete this->t->root();
    delete twin.root();
    delete twin1.root();
}

template <typename T>
Treap<T> foo() {
    Treap<T> t{42, 21};
    if (t.isValid()) {
        return t;
    }
    return Treap<T>{0, 0};
}

TYPED_TEST_P(TreapTest, TreapMovesFromFoo) {
    Treap<TypeParam> robber = foo<TypeParam>();
    delete robber.root();
    delete this->t->root();
}

TYPED_TEST_P(TreapTest, TreapMovesWithStdMove) {
    Treap<TypeParam> victim{42, 21};
    Treap<TypeParam> robber = std::move(victim);
    delete robber.root();
    delete this->t->root();
}

TYPED_TEST_P(TreapTest, TreapMoveAssignsFromFoo) {
    Treap<TypeParam> robber{7, 8};
    robber = foo<TypeParam>();
    ASSERT_EQ(robber.root()->key_, 21);
    delete robber.root();
    delete this->t->root();
}

TYPED_TEST_P(TreapTest, TreapMoveAssignsStdMove) {
    Treap<TypeParam> victim{42, 21};
    Treap<TypeParam> robber{7, 8};
    robber = std::move(victim);
    ASSERT_EQ(robber.root()->key_, 21);
    delete robber.root();
    delete this->t->root();
}

REGISTER_TYPED_TEST_SUITE_P(TreapTest, TreapBuilds, TreapSplits, TreapAssigns,
                            TreapCopyConstructor,
                            TreapInserts,
                            TreapMerges16,
                            TreapMerges20,
                            TreapMergesWithEmptyL,
                            TreapMergesWithEmptyR,
                            TreapMoveAssignsFromFoo,
                            TreapMoveAssignsStdMove,
                            TreapMovesFromFoo,
                            TreapMovesWithStdMove,
                            TreapRemoves,
                            TreapRemovesAll,
                            TreapRemovesNull,
                            TreapRemovesRightmost,
                            TreapSplitsWithEmptyLeft,
                            TreapSplitsWithEmptyRight);

using MyTypes = ::testing::Types<int, Foo>;
INSTANTIATE_TYPED_TEST_SUITE_P(My, TreapTest, MyTypes);
