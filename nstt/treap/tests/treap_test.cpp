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
        delete t->root_;
        delete t;
        delete defmap;
    }
};

TEST_F(TreapTest, TreapBuilds) {
    t->print();
    TreapNode *eight = t->find(8);
    std::pair<Treap *, Treap *> *two = t->split(20);
    ASSERT_EQ(isValid(t->root_), true);
}
