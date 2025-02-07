#include <gtest/gtest.h>
#include "../src/treap/treap.hpp"
#include <iostream>
#include <map>

using namespace std;

class TreapTest : public testing::Test {
protected:
    map<int, int> *initMap;
    Treap *t;

    bool isValid(TreapNode *node) {
        if (!node)
            return true;
        if (node->left() && (node->left()->key() > node->key() || node->left()->priority() < node->priority()))
            return false;
        if (node->right() && (node->right()->key() < node->key() || node->right()->priority() < node->priority()))
            return false;
        return isValid(node->left()) && isValid(node->right());
    }

    TreapTest() {
        initMap = new map<int, int> {
            {8, 10},
            {12, 8},
            {14, 14},
            {15, 4},
            {18, 9},
            {23, 6},
            {24, 15},
            {25, 11},
        };
        t = new Treap(initMap);
    }
    ~TreapTest() {
        delete initMap;
        delete t;
    }
};

TEST_F(TreapTest, TreapBuilds) {
    ASSERT_EQ(isValid(t->root()), true);
    delete t->root();
}

TEST_F(TreapTest, TreapSplits) {
    std::pair<Treap *, Treap *> *two = t->split(20);

    ASSERT_EQ(isValid(two->first->root()), true);
    ASSERT_EQ(isValid(two->second->root()), true);

    delete two->first->root();
    delete two->second->root();

    delete two->first;
    delete two->second;
    delete two;
}

TEST_F(TreapTest, TreapSplitsWithEmptyLeft) {
    std::pair<Treap *, Treap *> *two = t->split(5);

    ASSERT_EQ(isValid(two->first->root()), true);
    ASSERT_EQ(isValid(two->second->root()), true);
    ASSERT_EQ(two->first->empty(), true);
    ASSERT_EQ(two->second->empty(), false);

    delete two->first->root();
    delete two->second->root();

    delete two->first;
    delete two->second;
    delete two;
}

TEST_F(TreapTest, TreapSplitsWithEmptyRight) {
    std::pair<Treap *, Treap *> *two = t->split(30);

    ASSERT_EQ(isValid(two->first->root()), true);
    ASSERT_EQ(isValid(two->second->root()), true);
    ASSERT_EQ(two->first->empty(), false);
    ASSERT_EQ(two->second->empty(), true);

    delete two->first->root();
    delete two->second->root();

    delete two->first;
    delete two->second;
    delete two;
}

TEST_F(TreapTest, TreapMerges20) {
    std::pair<Treap *, Treap *> *two = t->split(20);
    two->first->merge(two->second);

    ASSERT_EQ(isValid(two->first->root()), true);

    delete two->first->root();
    delete two->first;
    delete two->second;
    delete two;
}

TEST_F(TreapTest, TreapMerges16) {
    std::pair<Treap *, Treap *> *two = t->split(16);
    two->first->merge(two->second);

    ASSERT_EQ(isValid(two->first->root()), true);

    delete two->first->root();
    delete two->first;
    delete two->second;
    delete two;
}

TEST_F(TreapTest, TreapMergesWithEmptyR) {
    std::pair<Treap *, Treap *> *two = t->split(300);
    two->first->merge(two->second);

    ASSERT_EQ(isValid(two->first->root()), true);

    delete two->first->root();
    delete two->first;
    delete two->second;
    delete two;
}

TEST_F(TreapTest, TreapMergesWithEmptyL) {
    std::pair<Treap *, Treap *> *two = t->split(2);
    two->first->merge(two->second);

    ASSERT_EQ(isValid(two->first->root()), true);

    delete two->first->root();
    delete two->first;
    delete two->second;
    delete two;
}

TEST_F(TreapTest, TreapMergesMisc0) {
    std::map<int, int> *init = new std::map<int, int>
    {
        {19, 15},
        {20, 13},
        {23, 6},
        {25, 11},
        {24, 15}
    };

    Treap t1 = Treap(new TreapNode(9, 18));
    Treap t2 = Treap(init);

    ASSERT_EQ(isValid(t1.root()), true);
    ASSERT_EQ(isValid(t2.root()), true);

    t1.merge(&t2);
    ASSERT_EQ(isValid(t1.root()), true);

    delete init;
    delete t1.root();
    delete t->root();
}

TEST_F(TreapTest, TreapInserts) {
    TreapNode *n = new TreapNode(7, 20);
    t->insert(n);
    ASSERT_EQ(isValid(t->root()), true);
    ASSERT_EQ(t->find(20), n);

    n = new TreapNode(5, 99);
    t->insert(n);
    ASSERT_EQ(isValid(t->root()), true);
    ASSERT_EQ(t->find(99), n);
    delete t->root();
}

TEST_F(TreapTest, TreapInsertsNull) {
    t->insert(nullptr);
    ASSERT_EQ(isValid(t->root()), true);
    delete t->root();
}

TEST_F(TreapTest, TreapRemoves) {
    t->remove(23);
    delete t->root();
}

TEST_F(TreapTest, TreapRemovesRightmost) {
    t->remove(24);

    ASSERT_EQ(isValid(t->root()), true);

    delete t->root();
}

TEST_F(TreapTest, TreapRemovesAll) {
    
    for (map<int, int>::const_iterator it = initMap->begin();
            it != initMap->end(); it++) {
        t->remove(it->first);
    }

    ASSERT_EQ(t->empty(), true);

    delete t->root();
}
