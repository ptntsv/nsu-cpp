#include <iostream>

#include "treap.hpp"

Treap::TreapNode::TreapNode() {}

Treap::TreapNode::TreapNode(int priority, int key)
    : key_(key), priority_(priority) {}

Treap::TreapNode::~TreapNode() {
    delete this->left_;
    delete this->right_;
}

bool Treap::TreapNode::operator==(const TreapNode &other) {
    return key_ == other.key_ && priority_ == other.priority_;
}

bool Treap::TreapNode::operator!=(const TreapNode &other) {
    return !(*this == other);
}

Treap::TreapNode::TreapNode(const TreapNode &other)
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

void Treap::TreapNode::hang(TreapNode **dst, TreapNode *newChild) {
    if (!dst)
        return;
    if (newChild)
        newChild->parent_ = this;
    *dst = newChild;
}

void Treap::TreapNode::tryToHang(TreapNode **root, TreapNode *newChild) {
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

void Treap::TreapNode::hangLeft(TreapNode *child) { hang(&left_, child); }
void Treap::TreapNode::hangRight(TreapNode *child) { hang(&right_, child); }

void Treap::TreapNode::detach() {
    if (parent_) {
        if (parent_->left_ == this)
            parent_->left_ = nullptr;
        else
            parent_->right_ = nullptr;
    }
    parent_ = nullptr;
}

void Treap::TreapNode::print() {
    if (left_)
        left_->print();
    std::cout << "Key: " << key_ << " " << priority_ << std::endl;
    if (right_)
        right_->print();
}
