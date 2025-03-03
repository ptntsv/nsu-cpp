#include "../src/treap/treap.hpp"

#include <gtest/gtest.h>

#include <iostream>
#include <map>

using namespace std;

class TreapTest : public testing::Test {
   protected:
    map<int, int>* initMap;
    Treap* t;

    bool isValid(Treap::TreapNode* node) {
        if (!node)
            return true;
        if (node->left_ && (node->left_->key_ > node->key_ ||
                            node->left_->priority_ < node->priority_))
            return false;
        if (node->right_ && (node->right_->key_ < node->key_ ||
                             node->right_->priority_ < node->priority_))
            return false;
        return isValid(node->left_) && isValid(node->right_);
    }
    TreapTest() {
        initMap = new map<int, int>{
            {8, 10}, {12, 8}, {14, 14}, {15, 4},
            {18, 9}, {23, 6}, {24, 15}, {25, 11},
        };
        t = new Treap(*initMap);
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
    std::pair<Treap*, Treap*> two = t->split(20);

    ASSERT_EQ(isValid(two.first->root()), true);
    ASSERT_EQ(isValid(two.second->root()), true);

    delete two.first->root();
    delete two.second->root();

    delete two.first;
    delete two.second;
}

TEST_F(TreapTest, TreapSplitsWithEmptyLeft) {
    std::pair<Treap*, Treap*> two = t->split(5);

    ASSERT_EQ(isValid(two.first->root()), true);
    ASSERT_EQ(isValid(two.second->root()), true);
    ASSERT_EQ(two.first->empty(), true);
    ASSERT_EQ(two.second->empty(), false);

    delete two.second->root();

    delete two.first;
    delete two.second;
}

TEST_F(TreapTest, TreapSplitsWithEmptyRight) {
    std::pair<Treap*, Treap*> two = t->split(30);

    ASSERT_EQ(isValid(two.first->root()), true);
    ASSERT_EQ(isValid(two.second->root()), true);
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

    ASSERT_EQ(isValid(two.first->root()), true);

    delete two.first->root();
    delete two.first;
    delete two.second;
}

TEST_F(TreapTest, TreapMerges16) {
    std::pair<Treap*, Treap*> two = t->split(16);
    two.first->merge(two.second);

    ASSERT_EQ(isValid(two.first->root()), true);

    delete two.first->root();
    delete two.first;
    delete two.second;
}

TEST_F(TreapTest, TreapMergesWithEmptyR) {
    std::pair<Treap*, Treap*> two = t->split(300);
    two.first->merge(two.second);

    ASSERT_EQ(isValid(two.first->root()), true);

    delete two.first->root();
    delete two.first;
    delete two.second;
}

TEST_F(TreapTest, TreapMergesWithEmptyL) {
    std::pair<Treap*, Treap*> two = t->split(2);
    two.first->merge(two.second);

    ASSERT_EQ(isValid(two.first->root()), true);

    delete two.first->root();
    delete two.first;
    delete two.second;
}

TEST_F(TreapTest, TreapMergesMisc0) {
    std::map<int, int>* init =
        new std::map<int, int>{{19, 15}, {20, 13}, {23, 6}, {25, 11}, {24, 15}};

    Treap t1 = Treap(9, 18);
    Treap t2 = Treap(*init);

    ASSERT_EQ(isValid(t1.root()), true);
    ASSERT_EQ(isValid(t2.root()), true);

    t1.merge(&t2);
    ASSERT_EQ(isValid(t1.root()), true);

    delete init;
    delete t1.root();
    delete t->root();
}

TEST_F(TreapTest, TreapInserts) {
    t->insert(7, 20);
    ASSERT_EQ(isValid(t->root()), true);
    ASSERT_EQ(t->find(20)->priority_, 7);

    t->insert(5, 99);
    ASSERT_EQ(isValid(t->root()), true);
    ASSERT_EQ(t->find(99)->priority_, 5);
    t->insert(42, 21);
    ASSERT_EQ(isValid(t->root()), true);
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

TEST_F(TreapTest, TreapCopyConstructor) {
    Treap twin{*t};
    ASSERT_TRUE(isValid(twin.root()));
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
