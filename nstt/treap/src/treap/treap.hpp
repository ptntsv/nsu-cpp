#pragma once

#include <assert.h>

#include <map>
#include <queue>

#define LOGS

#ifdef LOGS
#include <iostream>
#endif

template <typename T>
class Treap {
    struct TreapNode {
        TreapNode* left_ = nullptr;
        TreapNode* right_ = nullptr;
        TreapNode* parent_ = nullptr;
        int priority_;
        T key_;
        TreapNode() {};
        TreapNode(int priority, T key) : key_(key), priority_(priority) {}
        TreapNode(const TreapNode& other)
            : key_(other.key_), priority_(other.priority_) {
            if (other.left_) {
                left_ = new TreapNode(*other.left_);
                left_->parent_ = this;
            }
            if (other.right_) {
                right_ = new TreapNode(*other.right_);
                right_->parent_ = this;
            }
        }

        TreapNode& operator=(const TreapNode& other) = delete;
        bool operator==(const TreapNode& other) {
            return key_ == other.key_ && priority_ == other.priority_;
        }
        bool operator!=(const TreapNode& other) { return !(*this == other); }
        ~TreapNode() {
            delete this->left_;
            delete this->right_;
        }

        void hang(TreapNode** dst, TreapNode* newChild) {
            if (!dst) return;
            if (newChild) newChild->parent_ = this;
            *dst = newChild;
        }

        void tryToHang(TreapNode** root, TreapNode* newChild) {
            if (!newChild) return;
            if (newChild->priority_ < priority_) {
                if (*root == this) {
                    newChild->hangLeft(this);
                    *root = newChild;
                    return;
                }
                parent_->tryToHang(root, newChild);
            } else {
                newChild->hangLeft(this->right_);
                this->hangRight(newChild);
            }
        }
        void hangLeft(TreapNode* child) { hang(&left_, child); }
        void hangRight(TreapNode* child) { hang(&right_, child); }
        void detach() {
            if (parent_) {
                if (parent_->left_ == this)
                    parent_->left_ = nullptr;
                else
                    parent_->right_ = nullptr;
            }
            parent_ = nullptr;
        }

        void print() {
            if (left_) left_->print();
            std::cout << "Key: " << key_ << " " << priority_ << std::endl;
            if (right_) right_->print();
        }
    };
    TreapNode* root_ = nullptr;
    bool isValidHelper(Treap::TreapNode* node) const;
    TreapNode* findHelper(T key, TreapNode* node) const;
    Treap(TreapNode* root) : root_(root) {}

public:
    class iterator;

    Treap<T>() = default;
    ~Treap<T>() = default;
    Treap<T>(int priority, T key) : root_(new TreapNode(priority, key)) {}
    Treap<T>(const std::map<T, int>& keysToPriority);

    Treap<T>(const Treap<T>& other) : root_(new TreapNode(*other.root_)) {}
    Treap<T>(Treap<T>&& other);

    Treap<T>& operator=(const Treap<T>& other) {
        if (this != &other) {
            delete root_;
            root_ = new TreapNode(*other.root_);
        }
        return *this;
    }
    Treap<T>& operator=(Treap<T>&& other) {
        if (this != &other) {
#ifdef LOGS
            std::cout << "Move assign operator is called!" << std::endl;
#endif
            std::swap(root_, other.root_);
            delete other.root_;
        }
        return *this;
    }
    bool operator==(const Treap<T>& other) {
        std::queue<TreapNode*> q1;
        std::queue<TreapNode*> q2;
        q1.push(root_);
        q2.push(other.root_);
        while (!q1.empty()) {
            if (q2.empty()) return false;
            TreapNode* n1 = q1.front();
            TreapNode* n2 = q2.front();
            q1.pop();
            q2.pop();
            if (*n1 != *n2) return false;
            if (n1->left_) q1.push(n1->left_);
            if (n1->right_) q1.push(n1->right_);

            if (n2->left_) q2.push(n2->left_);
            if (n2->right_) q2.push(n2->right_);
        }
        if (!q2.empty()) return false;
        return true;
    }

    std::pair<Treap<T>*, Treap<T>*> split(T k);
    void merge(Treap<T>* t);
    void insert(int priority, T key);
    void remove(T key);
    TreapNode* find(T key);

    TreapNode* root() { return root_; }

    bool empty() const { return root_ == nullptr; }
    bool isValid() const { return isValidHelper(root_); }
    void print() const {
        if (!empty()) root_->print();
    }

    iterator begin() const;
    iterator end() const;
};
#include "treap.tcc"
