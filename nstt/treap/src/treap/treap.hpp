#pragma once

#include <gtest/gtest_prod.h>

#include <map>

class Treap {
    struct TreapNode {
        TreapNode* left_ = nullptr;
        TreapNode* right_ = nullptr;
        TreapNode* parent_ = nullptr;
        int priority_;
        int key_;
        TreapNode();
        TreapNode(int priority, int key);
        TreapNode(const TreapNode& other);
        TreapNode& operator=(const TreapNode& other) = delete;
        bool operator==(const TreapNode& other);
        bool operator!=(const TreapNode& other);
        ~TreapNode();

        void hang(TreapNode** dst, TreapNode* newChild);
        void tryToHang(TreapNode** root, TreapNode* newChild);
        void hangLeft(TreapNode* child);
        void hangRight(TreapNode* child);
        void detach();

        void print();
    };
    TreapNode* root_;
    TreapNode* find_helper(int key, TreapNode* node);
    Treap(TreapNode* root);
    friend class TreapTest;

   public:
    Treap();
    Treap(int priority, int key);
    Treap(std::map<int, int>& keysToPriority);
    Treap(const Treap& other);
    Treap& operator=(const Treap& other);
    bool operator==(const Treap& other);

    TreapNode* root() { return root_; }
    bool empty();
    bool isValid(Treap::TreapNode* node);
    std::pair<Treap*, Treap*> split(int k);
    void merge(Treap*);
    void insert(int priority, int key);
    void remove(int k);
    TreapNode* find(int key);
    void print();
};
