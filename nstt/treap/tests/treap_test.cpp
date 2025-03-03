#include "../src/treap/treap.hpp"

#include <gtest/gtest.h>

#include <iostream>
#include <map>

using namespace std;

class TreapTest : public testing::Test {
   protected:
    map<int, int> initMap;
    Treap* t;

    TreapTest()
        : initMap({
              {8, 10},
              {12, 8},
              {14, 14},
              {15, 4},
              {18, 9},
              {23, 6},
              {24, 15},
              {25, 11},
          }),
          t{new Treap(initMap)} {}

    ~TreapTest() { delete t; }
};

TEST_F(TreapTest, TreapBuilds) {
    ASSERT_EQ(t->isValid(), true);
    delete t->root();
}

TEST_F(TreapTest, TreapSplits) {
    std::pair<Treap*, Treap*> two = t->split(20);

    ASSERT_EQ(two.first->isValid(), true);
    ASSERT_EQ(two.second->isValid(), true);

    delete two.first->root();
    delete two.second->root();

    delete two.first;
    delete two.second;
}

TEST_F(TreapTest, TreapSplitsWithEmptyLeft) {
    std::pair<Treap*, Treap*> two = t->split(5);

    ASSERT_EQ(two.first->isValid(), true);
    ASSERT_EQ(two.second->isValid(), true);
    ASSERT_EQ(two.first->empty(), true);
    ASSERT_EQ(two.second->empty(), false);

    delete two.second->root();

    delete two.first;
    delete two.second;
}

TEST_F(TreapTest, TreapSplitsWithEmptyRight) {
    std::pair<Treap*, Treap*> two = t->split(30);

    ASSERT_EQ(two.first->isValid(), true);
    ASSERT_EQ(two.second->isValid(), true);
    ASSERT_EQ(two.first->empty(), false);
    ASSERT_EQ(two.second->empty(), true);

    delete two.first->root();
    delete two.second->root();

    delete two.first;
    delete two.second;
}

TEST_F(TreapTest, TreapMerges20) {
    std::pair<Treap*, Treap*> two = t->split(20);
    two.first->merge(two.second);

    ASSERT_EQ(two.first->isValid(), true);

    delete two.first->root();
    delete two.first;
    delete two.second;
}

TEST_F(TreapTest, TreapMerges16) {
    std::pair<Treap*, Treap*> two = t->split(16);
    two.first->merge(two.second);

    ASSERT_EQ(two.first->isValid(), true);

    delete two.first->root();
    delete two.first;
    delete two.second;
}

TEST_F(TreapTest, TreapMergesWithEmptyR) {
    std::pair<Treap*, Treap*> two = t->split(300);
    two.first->merge(two.second);

    ASSERT_EQ(two.first->isValid(), true);

    delete two.first->root();
    delete two.first;
    delete two.second;
}

TEST_F(TreapTest, TreapMergesWithEmptyL) {
    std::pair<Treap*, Treap*> two = t->split(2);
    two.first->merge(two.second);

    ASSERT_EQ(two.first->isValid(), true);

    delete two.first->root();
    delete two.first;
    delete two.second;
}

TEST_F(TreapTest, TreapMergesMisc0) {
    std::map<int, int>* init =
        new std::map<int, int>{{19, 15}, {20, 13}, {23, 6}, {25, 11}, {24, 15}};

    Treap t1 = Treap(9, 18);
    Treap t2 = Treap(*init);

    ASSERT_EQ(t1.isValid(), true);
    ASSERT_EQ(t2.isValid(), true);

    t1.merge(&t2);
    ASSERT_EQ(t1.isValid(), true);

    delete init;
    delete t1.root();
    delete t->root();
}

TEST_F(TreapTest, TreapInserts) {
    t->insert(7, 20);
    ASSERT_EQ(t->isValid(), true);
    ASSERT_EQ(t->find(20)->priority_, 7);

    t->insert(5, 99);
    ASSERT_EQ(t->isValid(), true);
    ASSERT_EQ(t->find(99)->priority_, 5);
    t->insert(42, 21);
    ASSERT_EQ(t->isValid(), true);
    ASSERT_EQ(t->find(21)->priority_, 42);
    delete t->root();
}

TEST_F(TreapTest, TreapRemoves) {
    t->remove(23);
    delete t->root();
}

TEST_F(TreapTest, TreapRemovesNull) {
    t->remove(228);
    delete t->root();
}

TEST_F(TreapTest, TreapRemovesRightmost) {
    t->remove(24);

    ASSERT_EQ(t->isValid(), true);

    delete t->root();
}

TEST_F(TreapTest, TreapRemovesAll) {
    for (map<int, int>::const_iterator it = initMap.begin();
         it != initMap.end(); it++) {
        t->remove(it->first);
    }

    ASSERT_EQ(t->empty(), true);

    delete t->root();
}

TEST_F(TreapTest, TreapCopyConstructor) {
    Treap twin{*t};
    ASSERT_TRUE(twin.isValid());
    ASSERT_TRUE(twin == *t);
    twin.insert(69, 96);
    ASSERT_FALSE(twin == *t);
    delete t->root();
    delete twin.root();
}

TEST_F(TreapTest, TreapAssigns) {
    Treap twin{*t};
    Treap twin1{5, 2};
    ASSERT_TRUE(twin == *t);
    twin = twin1;
    ASSERT_TRUE(twin == twin1);
    ASSERT_FALSE(twin == *t);
    delete t->root();
    delete twin.root();
    delete twin1.root();
}

Treap foo() {
    Treap t{42, 21};
    if (t.isValid()) {
        return t;
    }
    return Treap{0, 0};
}

TEST_F(TreapTest, TreapMovesFromFoo) {
    Treap robber = foo();
    delete robber.root();
    delete t->root();
}

TEST_F(TreapTest, TreapMovesWithStdMove) {
    Treap victim{42, 21};
    Treap robber = std::move(victim);
    delete robber.root();
    delete t->root();
}

TEST_F(TreapTest, TreapMoveAssignsFromFoo) {
    Treap robber{7, 8};
    robber = foo();
    ASSERT_EQ(robber.root()->key_, 21);
    delete robber.root();
    delete t->root();
}

TEST_F(TreapTest, TreapMoveAssignsStdMove) {
    Treap victim{42, 21};
    Treap robber{7, 8};
    robber = std::move(victim);
    ASSERT_EQ(robber.root()->key_, 21);
    delete robber.root();
    delete t->root();
}
