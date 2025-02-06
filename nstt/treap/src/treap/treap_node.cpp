#include "treap.hpp"

#include <iostream>

TreapNode::TreapNode() {
    left_ = nullptr;
    right_ = nullptr;
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
            newChild->hang(&newChild->left_, this);
            *root = newChild;
            return;
        }
        parent_->tryToHang(root, newChild);
    } else {
        newChild->hang(&newChild->left_, this->right_);
        this->hang(&this->right_, newChild);
    }
}

void TreapNode::hangLeft(TreapNode *child) {
    hang(&left_, child);
}
void TreapNode::hangRight(TreapNode *child) {
    hang(&right_, child);
}

void TreapNode::print() {
    if (left_)
        left_->print();
    std::cout << "Key: " << key_ << " " << priority_ << std::endl;
    if (right_)
        right_->print();
}
