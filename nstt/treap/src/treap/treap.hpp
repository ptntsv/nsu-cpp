#pragma once

#include <gtest/gtest_prod.h>
#include <map>


class TreapNode {
    TreapNode *left_;
    TreapNode *right_;
    TreapNode *parent_;
    int priority_;
    int key_;


public:
    TreapNode();
    TreapNode(int priority, int key);
    ~TreapNode();

    void hang(TreapNode **dst, TreapNode *newChild);
    void tryToHang(TreapNode **root, TreapNode *newChild);
    void hangLeft(TreapNode *child);
    void hangRight(TreapNode *child);
    void detach();

    void print();

    int key() {
        return key_;
    }

    int priority() {
        return priority_;
    }

    // TreapNode const * left() {
    TreapNode *left() {
        return left_;
    }
    // TreapNode const * right() {
    TreapNode *right() {
        return right_;
    }
    // TreapNode const * parent() {
    TreapNode *parent() {
        return parent_;
    }
};

class Treap {
private:
    TreapNode *root_;
    TreapNode * find_helper(int key, TreapNode *node);
    std::pair<Treap *, Treap *> **split_helper(int k);
    friend class TreapTest;
public:
    Treap();
    Treap(TreapNode *root);
    Treap(std::map<int, int> *keysToPriority);
    TreapNode *root() { return root_; }
    TreapNode *find(int key);
    std::pair<Treap *, Treap *> * split(int k);
    void merge(Treap *);
    void insert(int priority, int key);
    void print();
};


