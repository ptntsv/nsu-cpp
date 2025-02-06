#include <gtest/gtest.h>
#include "../src/treap/treap.hpp"
#include <iostream>
#include <map>

using namespace std;

class TreapTest : public testing::Test {
protected:
    map<int, int> *defmap;
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
        defmap = new map<int, int> {
            {8, 10},
            {12, 8},
            {14, 14},
            {15, 4},
            {18, 9},
            {23, 6},
            {24, 15},
            {25, 11},
        };
        t = new Treap(defmap);
    }
    ~TreapTest() {
        delete defmap;
    }
};

TEST_F(TreapTest, TreapBuilds) {
    ASSERT_EQ(isValid(t->root()), true);
    delete t->root();
    delete t;
}

TEST_F(TreapTest, TreapSplits) {
    std::pair<Treap *, Treap *> *two = t->split(20);
    if (two->first) {
        delete two->first->root();
        delete two->first;
    }
    if (two->second) {
        delete two->second->root();
        delete two->second;
    }
    delete two;
}

TEST_F(TreapTest, TreapMerges20) {
    std::pair<Treap *, Treap *> *two = t->split(20);
    two->first->merge(two->second);

    if (two->first) {
        delete two->first->root();
        delete two->first;
    }
    if (two->second) {
        delete two->second;
    }
    delete two;
}

TEST_F(TreapTest, TreapMerges16) {
    std::pair<Treap *, Treap *> *two = t->split(16);
    two->first->merge(two->second);

    two->first->print();
    if (two->first) {
        delete two->first->root();
        delete two->first;
    }
    if (two->second) {
        delete two->second;
    }
    delete two;
}
