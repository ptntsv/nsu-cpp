#pragma once

#include <gtest/gtest_prod.h>

#include <map>

#define LOGS

#ifdef LOGS
#include <iostream>
#endif

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
    bool isValidHelper(Treap::TreapNode* node);
    TreapNode* findHelper(int key, TreapNode* node);
    Treap(TreapNode* root);

   public:
    Treap();
    Treap(int priority, int key);
    Treap(const std::map<int, int>& keysToPriority);
    Treap(const Treap& other);
    Treap(Treap&& other);

    Treap& operator=(const Treap& other);
    Treap& operator=(Treap&& other);
    bool operator==(const Treap& other);

    TreapNode* root() { return root_; }
    bool empty();
    bool isValid();
    std::pair<Treap*, Treap*> split(int k);
    void merge(Treap*);
    void insert(int priority, int key);
    void remove(int k);
    TreapNode* find(int key);
    void print();
};
