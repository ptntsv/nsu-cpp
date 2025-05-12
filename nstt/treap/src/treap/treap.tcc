#include <queue>
#include <stack>

template <typename T>
void Treap<T>::merge(std::shared_ptr<Treap<T>> t)
{
    if (t->empty()) return;
    if (empty())
    {
        root_ = t->root_;
        return;
    }
    if (root_->priority_ < t->root_->priority_)
    {
        Treap subTreap(root_->right_);
        subTreap.merge(t);
        root_->hangRight(subTreap.root_);
        return;
    }
    Treap subTreap(t->root_->left_);
    this->merge(std::make_shared<Treap>(subTreap));
    t->root_->hangLeft(root_);
    root_ = t->root_;
}

template <typename T>
void Treap<T>::insert(int priority, T key)
{
    if (empty())
    {
        root_ = std::make_shared<TreapNode>(priority, key);
        return;
    }
    auto ts = split(key);
    Treap sub(priority, key);
    ts.first->merge(std::make_shared<Treap>(sub));
    ts.first->merge(ts.second);
    root_ = ts.first->root_;
}

template <typename T>
void Treap<T>::remove(T k)
{
    auto ts = split(k);
    if (ts.second->empty())
    {
        return;
    }
    std::shared_ptr<TreapNode> needle = ts.second->find(k);
    assert(!needle->left_ &&
        (!needle->parent_.lock() || needle->parent_.lock()->left_ == needle));
    if (!needle->parent_.lock())
    {
        ts.second->root_ = needle->right_;
        if (needle->right_) needle->right_->detach();
    }
    else
    {
        needle->parent_.lock()->hangLeft(needle->right_);
    }
    needle->hangRight(nullptr);
    ts.first->merge(ts.second);
    root_ = ts.first->root_;

    assert(!needle->left_ && !needle->right_);
};

template <typename T>
std::shared_ptr<typename Treap<T>::TreapNode> Treap<T>::findHelper(
    T key, std::shared_ptr<TreapNode> node) const
{
    if (!node || node->key_ == key) return node;
    if (key < node->key_)
        return findHelper(key, node->left_);
    return findHelper(key, node->right_);
}

template <typename T>
std::shared_ptr<typename Treap<T>::TreapNode> Treap<T>::find(T key)
{
    return findHelper(key, root_);
}

template <typename T>
std::pair<std::shared_ptr<Treap<T>>, std::shared_ptr<Treap<T>>> Treap<T>::split(T k)
{
    if (root_->key_ < k)
    {
        if (root_->right_)
        {
            auto R =
                Treap(root_->right_).split(k);
            if (!R.second->empty())
            {
                R.second->root_->detach();
            }
            if (!R.first->empty())
            {
                R.first->root_->detach();
                root_->hangRight(R.first->root_);
            }
            // delete R.first;
            R.first = std::make_shared<Treap>(root_);
            return R;
        }
        return std::pair<std::shared_ptr<Treap>, std::shared_ptr<Treap>>(std::make_shared<Treap>(root_),
                                                                         std::make_shared<Treap>());
    }
    if (root_->left_)
    {
        std::pair<std::shared_ptr<Treap>, std::shared_ptr<Treap>> L = Treap(root_->left_).split(k);
        if (!L.first->empty())
        {
            L.first->root_->detach();
        }
        if (!L.second->empty())
        {
            L.second->root_->detach();
            root_->hangLeft(L.second->root_);
        }
        // delete L.second;
        L.second = std::make_shared<Treap>(root_);
        return L;
    }
    return std::pair<std::shared_ptr<Treap>, std::shared_ptr<Treap>>(std::make_shared<Treap>(),
                                                                     std::make_shared<Treap>(root_));
}

template <typename T>
Treap<T>::Treap(const std::map<T, int>& keysToPriority)
{
    std::shared_ptr<TreapNode> prevNode;
    for (typename std::map<T, int>::const_iterator it = keysToPriority.begin();
         it != keysToPriority.end(); it++)
    {
        auto to_add = std::make_shared<TreapNode>(it->second, it->first);
        if (it == keysToPriority.begin())
            root_ = to_add;
        else
            prevNode->tryToHang(&root_, to_add);
        prevNode = to_add;
    }
}

template <typename T>
Treap<T>::Treap(std::initializer_list<T> keys)
{
    for (auto&& k : keys)
    {
        insert(get_unused_prior(), k);
    }
}

template <typename T>
Treap<T>::Treap(Treap<T>&& other)
{
#ifdef LOGS
    std::cout << "Move constructor is called!" << std::endl;
#endif
    root_ = other.root_;
    other.root_ = nullptr;
}

template <typename T>
bool Treap<T>::is_valid_helper(
    std::shared_ptr<typename Treap::TreapNode> node) const
{
    if (!node) return true;
    if (node->left_ && (node->left_->key_ > node->key_ ||
        node->left_->priority_ < node->priority_))
        return false;
    if (node->right_ && (node->right_->key_ < node->key_ ||
        node->right_->priority_ < node->priority_))
        return false;
    return is_valid_helper(node->left_) && is_valid_helper(node->right_);
}

template <typename T>
class Treap<T>::iterator
{
    std::shared_ptr<TreapNode> leftmost(
        std::shared_ptr<TreapNode> subtree_root)
    {
        if (subtree_root->left_) return leftmost(subtree_root->left_);
        if (subtree_root->right_) return leftmost(subtree_root->right_);
        return subtree_root;
    }

    std::shared_ptr<TreapNode> current_ = nullptr;
    bool is_end_ = false;

    std::shared_ptr<TreapNode> next()
    {
        if (!current_) return nullptr;
        auto p_subtree = current_->parent_.lock();
        if (!p_subtree)
        {
            is_end_ = true;
            return nullptr;
        }
        if (current_ == p_subtree->left_ && p_subtree->right_)
            return leftmost(p_subtree->right_);
        return p_subtree;
    }

public:
    iterator() = default;

    iterator(bool is_end) : is_end_(is_end)
    {
    }

    iterator(std::shared_ptr<TreapNode> ptr) { current_ = leftmost(ptr); }
    TreapNode& operator*() const { return *current_; }
    std::shared_ptr<TreapNode> operator->() const { return current_; }

    bool operator!=(const iterator& other) const
    {
        return !(is_end_ && other.is_end_);
    }

    std::shared_ptr<TreapNode> operator++() { return current_ = next(); }
};

template <typename T>
typename Treap<T>::iterator Treap<T>::begin() const
{
    if (empty()) return end();
    return iterator(root_);
}

template <typename T>
typename Treap<T>::iterator Treap<T>::end() const
{
    return iterator(true);
}
