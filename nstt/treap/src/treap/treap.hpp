#pragma once

#include <assert.h>
#include <gtest/gtest_prod.h>

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
            if (!dst)
                return;
            if (newChild)
                newChild->parent_ = this;
            *dst = newChild;
        }

        void tryToHang(TreapNode** root, TreapNode* newChild) {
            if (!newChild)
                return;
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
            if (left_)
                left_->print();
            std::cout << "Key: " << key_ << " " << priority_ << std::endl;
            if (right_)
                right_->print();
        }
    };
    TreapNode* root_ = nullptr;
    bool isValidHelper(Treap::TreapNode* node) {
        if (!node)
            return true;
        if (node->left_ && (node->left_->key_ > node->key_ ||
                            node->left_->priority_ < node->priority_))
            return false;
        if (node->right_ && (node->right_->key_ < node->key_ ||
                             node->right_->priority_ < node->priority_))
            return false;
        return isValidHelper(node->left_) && isValidHelper(node->right_);
    }
    TreapNode* findHelper(T key, TreapNode* node) {
        if (!node || node->key_ == key)
            return node;
        if (key < node->key_)
            return findHelper(key, node->left_);
        else
            return findHelper(key, node->right_);
    }
    Treap(TreapNode* root) : root_(root) {}

public:
    Treap<T>() = default;
    ~Treap<T>() = default;
    Treap<T>(int priority, T key) : root_(new TreapNode(priority, key)) {}
    Treap<T>(const std::map<T, int>& keysToPriority) {
        TreapNode* prevNode;
        for (typename std::map<T, int>::const_iterator it =
                 keysToPriority.begin();
             it != keysToPriority.end(); it++) {
            TreapNode* toAdd = new TreapNode(it->second, it->first);
            if (it == keysToPriority.begin()) {
                root_ = toAdd;
            } else {
                prevNode->tryToHang(&root_, toAdd);
            }
            prevNode = toAdd;
        }
    }
    Treap<T>(const Treap<T>& other) : root_(new TreapNode(*other.root_)) {}
    Treap<T>(Treap<T>&& other) {
#ifdef LOGS
        std::cout << "Move constructor is called!" << std::endl;
#endif
        root_ = other.root_;
        other.root_ = nullptr;
    }

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
            if (q2.empty())
                return false;
            TreapNode* n1 = q1.front();
            TreapNode* n2 = q2.front();
            q1.pop();
            q2.pop();
            if (*n1 != *n2)
                return false;
            if (n1->left_)
                q1.push(n1->left_);
            if (n1->right_)
                q1.push(n1->right_);

            if (n2->left_)
                q2.push(n2->left_);
            if (n2->right_)
                q2.push(n2->right_);
        }
        if (!q2.empty())
            return false;
        return true;
    }

    TreapNode* root() { return root_; }
    bool empty() { return root_ == nullptr; }
    bool isValid() { return isValidHelper(root_); }
    std::pair<Treap<T>*, Treap<T>*> split(T k) {
        if (root_->key_ < k) {
            if (root_->right_) {
                std::pair<Treap<T>*, Treap<T>*> R =
                    Treap<T>(root_->right_).split(k);
                if (!R.second->empty()) {
                    R.second->root_->detach();
                }
                if (!R.first->empty()) {
                    R.first->root_->detach();
                    root_->hangRight(R.first->root_);
                }
                delete R.first;
                R.first = new Treap<T>(root_);
                return R;
            }
            return std::pair<Treap<T>*, Treap<T>*>(new Treap<T>(root_),
                                                   new Treap<T>());
        }
        if (root_->left_) {
            std::pair<Treap<T>*, Treap<T>*> L = Treap<T>(root_->left_).split(k);
            if (!L.first->empty()) {
                L.first->root_->detach();
            }
            if (!L.second->empty()) {
                L.second->root_->detach();
                root_->hangLeft(L.second->root_);
            }
            delete L.second;
            L.second = new Treap<T>(root_);
            return L;
        }
        return std::pair<Treap<T>*, Treap<T>*>(new Treap<T>(),
                                               new Treap<T>(root_));
    }
    void merge(Treap* t) {
        if (t->empty())
            return;
        if (empty()) {
            root_ = t->root_;
            return;
        }
        if (root_->priority_ < t->root_->priority_) {
            Treap subTreap{root_->right_};
            subTreap.merge(t);
            root_->hangRight(subTreap.root_);
            return;
        }
        Treap subTreap{t->root_->left_};
        merge(&subTreap);
        t->root_->hangLeft(root_);
        root_ = t->root_;
    }
    void insert(int priority, T key) {
        std::pair<Treap*, Treap*> ts = split(key);
        Treap sub{priority, key};
        ts.first->merge(&sub);
        ts.first->merge(ts.second);
        root_ = ts.first->root_;
        delete ts.first;
        delete ts.second;
    }
    void remove(T k) {
        std::pair<Treap*, Treap*> ts = split(k);
        if (ts.second->empty()) {
            delete ts.first;
            delete ts.second;
            return;
        }
        TreapNode* needle = ts.second->find(k);
        assert(!needle->left_ &&
               (!needle->parent_ || needle->parent_->left_ == needle));
        if (!needle->parent_) {
            ts.second->root_ = needle->right_;
            if (needle->right_)
                needle->right_->detach();
        } else {
            needle->parent_->hangLeft(needle->right_);
        }
        needle->hangRight(nullptr);
        ts.first->merge(ts.second);
        root_ = ts.first->root_;

        assert(!needle->left_ && !needle->right_);
        delete needle;
        delete ts.first;
        delete ts.second;
    };
    TreapNode* find(T key) { return findHelper(key, root_); }
    void print() {
        if (!empty())
            root_->print();
    }
};
