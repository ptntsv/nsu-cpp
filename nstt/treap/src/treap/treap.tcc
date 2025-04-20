#include <queue>
#include <stack>

template <typename T>
void Treap<T>::merge(Treap<T>* t) {
    if (t->empty()) return;
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
template <typename T>
void Treap<T>::insert(int priority, T key) {
    std::pair<Treap*, Treap*> ts = split(key);
    Treap sub{priority, key};
    ts.first->merge(&sub);
    ts.first->merge(ts.second);
    root_ = ts.first->root_;
    delete ts.first;
    delete ts.second;
}

template <typename T>
void Treap<T>::remove(T k) {
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
        if (needle->right_) needle->right_->detach();
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

template <typename T>
typename Treap<T>::TreapNode* Treap<T>::findHelper(
    T key, typename Treap<T>::TreapNode* node) const {
    if (!node || node->key_ == key) return node;
    if (key < node->key_)
        return findHelper(key, node->left_);
    else
        return findHelper(key, node->right_);
}

template <typename T>
typename Treap<T>::TreapNode* Treap<T>::find(T key) {
    return findHelper(key, root_);
}

template <typename T>
std::pair<Treap<T>*, Treap<T>*> Treap<T>::split(T k) {
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
    return std::pair<Treap<T>*, Treap<T>*>(new Treap<T>(), new Treap<T>(root_));
}

template <typename T>
Treap<T>::Treap(const std::map<T, int>& keysToPriority) {
    TreapNode* prevNode;
    for (typename std::map<T, int>::const_iterator it = keysToPriority.begin();
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

template <typename T>
Treap<T>::Treap(Treap<T>&& other) {
#ifdef LOGS
    std::cout << "Move constructor is called!" << std::endl;
#endif
    root_ = other.root_;
    other.root_ = nullptr;
}

template <typename T>
bool Treap<T>::isValidHelper(Treap::TreapNode* node) const {
    if (!node) return true;
    if (node->left_ && (node->left_->key_ > node->key_ ||
                        node->left_->priority_ < node->priority_))
        return false;
    if (node->right_ && (node->right_->key_ < node->key_ ||
                         node->right_->priority_ < node->priority_))
        return false;
    return isValidHelper(node->left_) && isValidHelper(node->right_);
}

template <typename T>
class Treap<T>::iterator {
    std::queue<TreapNode*> queue;
    void update_queue(TreapNode* node) {
        if (!node) return;
        if (node->left_) queue.push(node->left_);
        if (node->right_) queue.push(node->right_);
    }
    TreapNode* curr() const {
        if (queue.empty()) return nullptr;
        return queue.front();
    }
    TreapNode* next() {
        if (queue.empty()) return nullptr;
        auto n = queue.front();
        queue.pop();
        return n;
    }
    bool is_end() const { return queue.empty(); }

public:
    iterator() = default;
    iterator(TreapNode* ptr) { queue.push(ptr); }
    TreapNode& operator*() const { return *curr(); }
    TreapNode* operator->() const { return curr(); }
    bool operator!=(const iterator& other) const {
        return !(is_end() && other.is_end());
    }
    TreapNode* operator++() {
        TreapNode* popped = next();
        update_queue(popped);
        return popped;
    }
};

template <typename T>
typename Treap<T>::iterator Treap<T>::begin() const {
    if (empty()) return end();
    return iterator(root_);
}

template <typename T>
typename Treap<T>::iterator Treap<T>::end() const {
    return iterator();
}
