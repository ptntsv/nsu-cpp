#pragma once

#include <assert.h>

#include <initializer_list>
#include <map>
#include <memory>
#include <queue>
#include <random>
#include <set>

#define LOGS

#ifdef LOGS
#include <iostream>
#endif

template <typename T>
class Treap : public std::enable_shared_from_this<Treap<T>> {
    struct TreapNode : std::enable_shared_from_this<TreapNode> {
        static std::shared_ptr<TreapNode> create_from_other(
            std::shared_ptr<TreapNode> other) {
            if (!other) { return nullptr; }
            auto to_return = std::make_shared<TreapNode>();
            to_return->priority_ = other->priority_;
            to_return->key_ = other->key_;
            if (other->left_) {
                to_return->left_ = create_from_other(other->left_);
                to_return->left_->parent_ = to_return;
            }
            if (other->right_) {
                to_return->right_ = create_from_other(other->right_);
                to_return->right_->parent_ = to_return;
            }
            return to_return;
        }

        std::shared_ptr<TreapNode> left_;
        std::shared_ptr<TreapNode> right_;
        std::weak_ptr<TreapNode> parent_;
        int priority_{};
        T key_;
        std::shared_ptr<TreapNode> getme() { return this->shared_from_this(); }

        TreapNode() = default;

        TreapNode(int priority, T key)
            : priority_(priority), key_(key) {
        }

        // TreapNode(const TreapNode& other)
        //     : priority_(other.priority_), key_(other.key_) {
        //     if (other.left_) {
        //         left_ = std::make_shared<TreapNode>(*other.left_);
        //         left_->parent_ = getme();
        //     }
        //     if (other.right_) {
        //         right_ = std::make_shared<TreapNode>(*other.right_);
        //         right_->parent_ = getme();
        //     }
        // }

        TreapNode& operator=(const TreapNode& other) = delete;

        bool operator==(const TreapNode& other) const {
            return key_ == other.key_ && priority_ == other.priority_;
        }

        bool operator!=(const TreapNode& other) { return !(*this == other); }

        ~TreapNode() = default;

        void tryToHang(std::shared_ptr<TreapNode>* root,
                       std::shared_ptr<TreapNode> newChild) {
            if (!newChild) return;
            if (newChild->priority_ < priority_) {
                if (*root == getme()) {
                    newChild->hangLeft(getme());
                    *root = newChild;
                    return;
                }
                if (auto parent = parent_.lock()) {
                    parent->tryToHang(root, newChild);
                }
            } else {
                newChild->hangLeft(this->right_);
                this->hangRight(newChild);
            }
        }

        void hangLeft(std::shared_ptr<TreapNode> child) {
            if (child) child->parent_ = getme();
            left_ = child;
        }

        void hangRight(std::shared_ptr<TreapNode> child) {
            if (child) child->parent_ = getme();
            right_ = child;
        }

        void detach() {
            if (auto parent = parent_.lock()) {
                if (parent->left_ == getme()) parent->left_ = nullptr;
                else parent->right_ = nullptr;
            }
            parent_.reset();
        }

        void print() {
            if (left_) left_->print();
            std::cout << "Key: " << key_ << " " << priority_ << std::endl;
            if (right_) right_->print();
        }
    };

    std::shared_ptr<TreapNode> root_;
    int min_prior = 0, max_prior = 100;
    std::set<int> used_priors = std::set<int>();

    bool is_valid_helper(std::shared_ptr<TreapNode> node) const;

    int get_unused_prior() {
        static std::uniform_int_distribution<> distr(min_prior, max_prior);
        static std::mt19937 gen;
        int p;
        do {
            p = distr(gen);
        } while (used_priors.count(p));
        used_priors.insert(p);
        if (used_priors.size() == max_prior) max_prior *= 2;
        return p;
    }

    std::shared_ptr<TreapNode> findHelper(
        T key, std::shared_ptr<TreapNode> node) const;

public:
    class iterator;

    Treap() = default;
    ~Treap() = default;
    Treap(int priority, T key) { insert(priority, key); }
    explicit Treap(const std::map<T, int>& keysToPriority);

    explicit Treap(std::shared_ptr<TreapNode> root) {
        root_ = root;
    }

    Treap(std::initializer_list<T>);

    Treap(const Treap& other) {
        if (!other.empty()) root_ = TreapNode::create_from_other(other.root_);
    }

    Treap(Treap&& other);

    Treap& operator=(const Treap& other) {
        if (this != &other) {
            root_ = std::make_shared<TreapNode>(*other.root_);
        }
        return *this;
    }

    Treap& operator=(Treap<T>&& other) {
        if (this != &other) {
#ifdef LOGS
            std::cout << "Move assign operator is called!" << std::endl;
#endif
            std::swap(root_, other.root_);
            // delete other.root_;
        }
        return *this;
    }

    bool operator==(const Treap<T>& other) const {
        std::queue<std::shared_ptr<TreapNode>> q1;
        std::queue<std::shared_ptr<TreapNode>> q2;
        q1.push(root_);
        q2.push(other.root_);
        while (!q1.empty()) {
            if (q2.empty()) return false;
            std::shared_ptr<TreapNode> n1 = q1.front();
            std::shared_ptr<TreapNode> n2 = q2.front();
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

    std::pair<std::shared_ptr<Treap>, std::shared_ptr<Treap>> split(T k);
    void merge(std::shared_ptr<Treap> t);
    void insert(int priority, T key);
    void remove(T key);
    std::shared_ptr<TreapNode> find(T key);

    std::shared_ptr<TreapNode> root() { return root_; }

    bool empty() const { return root_ == nullptr; }
    bool is_valid() const { return is_valid_helper(root_); }

    void print() const {
        if (!empty()) root_->print();
    }

    iterator begin() const;
    iterator end() const;
};

#include "treap.tcc"
