#include "treap.hpp"

#include <cstdlib>
#include <iostream>
#include <map>

using namespace std;

void Treap::insert(int priority, int key) {
}

Treap::Treap() {
    root_ = nullptr;
}

Treap::Treap(TreapNode *root) {
    root_ = root;
}

Treap::Treap(map<int, int> *keyToPriority) : Treap() {
    TreapNode *prevNode;
    for (map<int, int>::const_iterator it = keyToPriority->begin();
            it != keyToPriority->end(); it++) {
        TreapNode *toAdd = new TreapNode(it->second, it->first);
        if (it == keyToPriority->begin()) {
            root_ = toAdd;
        } else {
            prevNode->tryToHang(&root_, toAdd);
        }
        prevNode = toAdd;
    }
}

void Treap::print() {
    root_->print();
}

TreapNode * Treap::find_helper(int key, TreapNode *node) {
    if (!node || node->key() == key)
        return node;
    if (key < node->key())
        return find_helper(key, node->left());
    else
        return find_helper(key, node->right());
}

TreapNode * Treap::find(int key) {
    return find_helper(key, root_);
}

std::pair<Treap *, Treap *> * Treap::split(int k) {
    if (root_->key() < k) {
        Treap *tmp = new Treap(root_->right());
        std::pair<Treap *, Treap *> *R = tmp->split(k);
        if (R->second) {
            R->second->root_->hangLeft(nullptr);
        }
        root_->hangRight(R->first->root_);
        std::pair<Treap *, Treap *> *res = new std::pair<Treap *, Treap *> {this, R->second};
        delete tmp;
        delete R;
        return res;
    }
    return nullptr;
}
