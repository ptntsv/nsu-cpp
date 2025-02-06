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

// Review it pls...
std::pair<Treap *, Treap *> ** Treap::split_helper(int k) {
    if (root_->key() < k) {
        std::pair<Treap *, Treap *> **R;
        if (root_->right()) {
            Treap *subTreap = new Treap(root_->right());
            R = subTreap->split_helper(k);
            if ((*R)->second) {
                (*R)->second->root_->detach();
                if ((*R)->first)
                    (*R)->first->root_->detach();
            }
            if ((*R)->first)
                root_->hangRight((*R)->first->root_);
            delete (*R)->first;
            (*R)->first = this;
            return R;
        }
        R = new std::pair<Treap *, Treap *> *;
        *R = new std::pair<Treap *, Treap *> (this, nullptr);
        return R;
    }
    std::pair<Treap *, Treap *> **L;
    if (root_->left()) {
        Treap *subTreap = new Treap(root_->left());
        L = subTreap->split_helper(k);
        if ((*L)->first) {
            (*L)->first->root_->detach();
            if ((*L)->second)
                (*L)->second->root_->detach();
        }
        if ((*L)->second)
            root_->hangLeft((*L)->second->root_);

        delete (*L)->second;
        (*L)->second = this;
        return L;
    }
    L = new std::pair<Treap *, Treap *> *;
    *L = new std::pair<Treap *, Treap *> (nullptr, this);
    return L;
}

std::pair<Treap *, Treap *> * Treap::split(int k) {
    std::pair<Treap *, Treap *> ** pres = split_helper(k);
    std::pair<Treap *, Treap *> * res = *pres;
    delete pres;
    return res;
}

void Treap::merge(Treap *t) {
    if (!root_) {
        root_ = t->root_;
    } else if(!t->root_) {
        return;
    }
    if (root_->priority() < t->root_->priority()) {
        Treap *subTreap = new Treap(root_->right());
        subTreap->merge(t);
        root_->hangRight(subTreap->root());
        delete subTreap;
        return;
    }
    if (!t->root_->left()) {
        t->root_->hangLeft(root_);
        root_ = t->root_;
        return;
    }
    Treap *subTreap = new Treap(t->root_->left());
    merge(subTreap);
    t->root_->left()->detach();
    root_ = t->root_;
    delete subTreap;
}
