#include "treap.hpp"

#include <iostream>

TreapNode::TreapNode() {
    left_ = nullptr;
    right_ = nullptr;
    parent_ = nullptr;
}

TreapNode::TreapNode(int priority, int key) : TreapNode() {
    key_ = key;
    priority_ = priority;
}

TreapNode::~TreapNode() {
    delete this->left_;
    delete this->right_;
}

void TreapNode::hang(TreapNode **dst, TreapNode *newChild) {
    if (!dst)
        return;
    if (newChild)
        newChild->parent_ = this;
    *dst = newChild;
}

void TreapNode::tryToHang(TreapNode **root, TreapNode *newChild) {
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

void TreapNode::hangLeft(TreapNode *child) {
    hang(&left_, child);
}
void TreapNode::hangRight(TreapNode *child) {
    hang(&right_, child);
}

void TreapNode::detach() {
    if (parent_) {
        if (parent_->left() == this)
            parent_->left_ = nullptr;
        else
            parent_->right_ = nullptr;
    }
    parent_ = nullptr;
}

void TreapNode::print() {
    if (left_)
        left_->print();
    std::cout << "Key: " << key_ << " " << priority_ << std::endl;
    if (right_)
        right_->print();
}
