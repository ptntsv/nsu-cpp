#include "treap.hpp"

#include <assert.h>

#include <cstdlib>
#include <queue>

bool Treap::isValid(Treap::TreapNode* node) {
    if (!node)
        return true;
    if (node->left_ && (node->left_->key_ > node->key_ ||
                        node->left_->priority_ < node->priority_))
        return false;
    if (node->right_ && (node->right_->key_ < node->key_ ||
                         node->right_->priority_ < node->priority_))
        return false;
    return isValid(node->left_) && isValid(node->right_);
}

void Treap::remove(int k) {
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
}
void Treap::insert(int priority, int key) {
    std::pair<Treap*, Treap*> ts = split(key);
    Treap sub{priority, key};
    ts.first->merge(&sub);
    ts.first->merge(ts.second);
    root_ = ts.first->root_;
    delete ts.first;
    delete ts.second;
}

Treap::Treap() : root_(nullptr) {}

Treap::Treap(TreapNode* root) : root_(root) {}

Treap::Treap(int priority, int key) : root_(new TreapNode(priority, key)) {}

Treap::Treap(const Treap& other) : root_(new TreapNode(*other.root_)) {}

Treap& Treap::operator=(const Treap& other) {
    if (this != &other) {
        delete root_;
        root_ = new TreapNode(*other.root_);
    }
    return *this;
}

bool Treap::operator==(const Treap& other) {
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

Treap::Treap(std::map<int, int>& keyToPriority) : Treap() {
    TreapNode* prevNode;
    for (std::map<int, int>::const_iterator it = keyToPriority.begin();
         it != keyToPriority.end(); it++) {
        TreapNode* toAdd = new TreapNode(it->second, it->first);
        if (it == keyToPriority.begin()) {
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

Treap::TreapNode* Treap::find_helper(int key, TreapNode* node) {
    if (!node || node->key_ == key)
        return node;
    if (key < node->key_)
        return find_helper(key, node->left_);
    else
        return find_helper(key, node->right_);
}

Treap::TreapNode* Treap::find(int key) { return find_helper(key, root_); }

std::pair<Treap*, Treap*> Treap::split(int k) {
    if (root_->key_ < k) {
        if (root_->right_) {
            std::pair<Treap*, Treap*> R = Treap(root_->right_).split(k);
            if (!R.second->empty()) {
                R.second->root_->detach();
            }
            if (!R.first->empty()) {
                R.first->root_->detach();
                root_->hangRight(R.first->root_);
            }
            delete R.first;
            R.first = new Treap(root_);
            return R;
        }
        return std::pair<Treap*, Treap*>(new Treap(root_), new Treap());
    }
    if (root_->left_) {
        std::pair<Treap*, Treap*> L = Treap(root_->left_).split(k);
        if (!L.first->empty()) {
            L.first->root_->detach();
        }
        if (!L.second->empty()) {
            L.second->root_->detach();
            root_->hangLeft(L.second->root_);
        }
        delete L.second;
        L.second = new Treap(root_);
        return L;
    }
    return std::pair<Treap*, Treap*>(new Treap(), new Treap(root_));
}

void Treap::merge(Treap* t) {
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

bool Treap::empty() { return root_ == nullptr; }
