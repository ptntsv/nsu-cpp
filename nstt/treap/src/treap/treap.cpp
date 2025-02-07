#include "treap.hpp"

#include <cstdlib>
#include <map>
#include <assert.h>

using namespace std;

void Treap::remove(int k) {
    std::pair<Treap *, Treap *> *ts = split(k);
    if (ts->second->empty())
        return;
    TreapNode *needle = ts->second->find(k);
    assert(
            !needle->left() &&
            (!needle->parent() || needle->parent()->left() == needle)
    );
    if (!needle->parent()) {
        ts->second->root_ = needle->right();
        if (needle->right())
            needle->right()->detach();
    } else {
        needle->parent()->hangLeft(needle->right());
    }
    needle->hangRight(nullptr);
    ts->first->merge(ts->second);
    root_ = ts->first->root();

    assert(!needle->left() && !needle->right());
    delete needle;
    delete ts->first;
    delete ts->second;
    delete ts;
}
void Treap::insert(TreapNode *node) {
    if (!node)
        return;
    std::pair<Treap *, Treap *> *ts = split(node->key());
    Treap *sub = new Treap(node);
    ts->first->merge(sub);
    ts->first->merge(ts->second);
    root_ = ts->first->root_;
    delete sub;
    delete ts->first;
    delete ts->second;
    delete ts;
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
    if (!empty())
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
// TODO: give preallocated pair as argument
std::pair<Treap *, Treap *> ** Treap::split_helper(int k) {
    if (root_->key() < k) {
        std::pair<Treap *, Treap *> **R;
        if (root_->right()) {
            Treap *subTreap = new Treap(root_->right());
            R = subTreap->split_helper(k);
            if (!(*R)->second->empty()) {
                (*R)->second->root_->detach();
                if (!(*R)->first->empty())
                    (*R)->first->root_->detach();
            }
            if (!(*R)->first->empty())
                root_->hangRight((*R)->first->root_);
            delete (*R)->first;
            (*R)->first = new Treap(root());
            delete subTreap;
            return R;
        }
        R = new std::pair<Treap *, Treap *> *;
        *R = new std::pair<Treap *, Treap *> (new Treap(root()), new Treap());
        return R;
    }
    std::pair<Treap *, Treap *> **L;
    if (root_->left()) {
        Treap *subTreap = new Treap(root_->left());
        L = subTreap->split_helper(k);
        if (!(*L)->first->empty()) {
            (*L)->first->root_->detach();
            if (!(*L)->second->empty())
                (*L)->second->root_->detach();
        }
        if (!(*L)->second->empty())
            root_->hangLeft((*L)->second->root_);

        delete (*L)->second;
        (*L)->second = new Treap(root());
        delete subTreap;
        return L;
    }
    L = new std::pair<Treap *, Treap *> *;
    *L = new std::pair<Treap *, Treap *> (new Treap(), new Treap(root()));
    return L;
}

std::pair<Treap *, Treap *> * Treap::split(int k) {
    std::pair<Treap *, Treap *> ** pres = split_helper(k);
    std::pair<Treap *, Treap *> * res = *pres;
    delete pres;
    return res;
}

void Treap::merge(Treap *t) {
    if (empty()) {
        root_ = t->root_;
        return;
    }
    if(t->empty())
        return;
    if (root_->priority() < t->root_->priority()) {
        Treap *subTreap = new Treap(root_->right());
        subTreap->merge(t);
        root_->hangRight(subTreap->root());
        delete subTreap;
        return;
    }
    Treap *subTreap = new Treap(t->root_->left());
    merge(subTreap);
    t->root_->hangLeft(root());
    root_ = t->root_;
    delete subTreap;
}

bool Treap::empty() {
    return root_ == nullptr;
} 
