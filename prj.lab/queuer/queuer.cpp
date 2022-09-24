#include <queuer/queuer.h>

#include <algorithm>
#include <cassert>

QueueR::QueueR(const QueueR& queueR)
    : root_(nullptr)
    , last_(nullptr)
    , size_(queueR.size_)
{
    root_ = copy_(queueR.root_, nullptr, queueR.last_);
}

QueueR::QueueR(QueueR&& queueR)
    : root_(queueR.root_)
    , last_(queueR.last_)
    , size_(queueR.size_)
{
    queueR.root_ = nullptr;
    queueR.last_ = nullptr;
    queueR.size_ = 0;
}

QueueR& QueueR::operator=(const QueueR& queueR) {
    if (&queueR == this) {
        return *this;
    }
    delete_(root_);
    last_ = nullptr;
    size_ = queueR.size_;
    root_ = copy_(queueR.root_, nullptr, queueR.last_);
    return *this;
}

QueueR& QueueR::operator=(QueueR&& queueR) {
    if (&queueR == this) {
        return *this;
    }
    if (root_ != nullptr) {
        delete_(root_);
    }
    root_ = queueR.root_;
    last_ = queueR.last_;
    size_ = queueR.size_;
    queueR.root_ = nullptr;
    queueR.last_ = nullptr;
    queueR.size_ = 0;
    return *this;
}

QueueR::el_t QueueR::top() const {
    assert(root_ != nullptr && size_ != 0);
    return root_->value;
}

void QueueR::pop() {
    assert(root_ != nullptr && size_ != 0);
    if (size_ == 1) {
        assert(last_ == root_ && root_->left == nullptr && root_->right == nullptr);
        delete root_;
        root_ = nullptr;
        last_ = nullptr;
        size_ = 0;
        return;
    }
    if (size_ == 2) {
        assert(root_->left != nullptr && root_->right == nullptr);
        root_->value = root_->left->value;
        delete root_->left;
        root_->left = nullptr;
        last_ = root_;
        size_ = 1;
        return;
    }
    remove_();
    sinkDown_();
}

void QueueR::push(const el_t value) {
    if (size_ == 0) {
        root_ = new Node{ value, nullptr, nullptr, nullptr };
        last_ = root_;
        ++size_;
        return;
    }
    append_(value);
    sinkUp_();
}

QueueR::~QueueR() {
    if (root_ != nullptr) {
        delete_(root_);
    }
}

void QueueR::append_(const el_t value) {
    const s_t newSize{ size_ + 1 };
    const s_t level{ log2_(newSize) };
    s_t rowSize{ 1 << level };
    s_t rowMid{ rowSize / 2 };
    s_t rowIndex{ newSize - rowSize };
    Node* node{ root_ };
    for (s_t i{ 1 }; i < level; ++i) {
        if (rowIndex < rowMid) {
            node = node->left;
        } else {
            node = node->right;
            rowIndex -= rowMid;
        }
        assert(node != nullptr);
        rowSize /= 2;
        rowMid = rowSize / 2;
    }
    assert(rowSize == 2 && rowIndex < 2);
    Node* newNode{ new Node{ value, nullptr, nullptr, node } };
    (rowIndex == 0 ? node->left : node->right) = newNode;
    last_ = newNode;
    size_ = newSize;
}

void QueueR::remove_() {
    assert(size_ > 1);
    root_->value = last_->value;
    (last_->top->left == last_ ? last_->top->left : last_->top->right) = nullptr;
    delete last_;
    last_ = nullptr;
    --size_;
    const s_t level{ log2_(size_) };
    s_t rowSize{ 1 << level };
    s_t rowMid{ rowSize / 2 };
    s_t rowIndex{ size_ - rowSize };
    Node* node{ root_ };
    for (s_t i{ 0 }; i < level; ++i) {
        if (rowIndex < rowMid) {
            node = node->left;
        } else {
            node = node->right;
            rowIndex -= rowMid;
        }
        assert(node != nullptr);
        rowSize /= 2;
        rowMid = rowSize / 2;
    }
    last_ = node;
}

void QueueR::sinkUp_() {
    Node* node{ last_ };
    while (node->top != nullptr) {
        if (node->value >= node->top->value) {
            return;
        }
        std::swap(node->value, node->top->value);
        node = node->top;
    }
}

void QueueR::sinkDown_() {
    assert(size_ > 1);
    Node* node { root_ };
    while (node != nullptr) {
        Node* left{ node->left };
        Node* right{ node->right };
        if (left == nullptr && right == nullptr) {
            return;
        }
        if (right == nullptr) {
            if (node->value > left->value) {
                std::swap(node->value, left->value);
            }
            return;
        }
        assert(left != nullptr);
        Node* min{ left->value < right->value ? left : right };
        if (min->value >= node->value) {
            return;
        }
        std::swap(min->value, node->value);
        node = min;
    }
    assert(false);
}

QueueR::s_t QueueR::log2_(const s_t value) {
    assert(value > 0);
    s_t ret{ -1 };
    for (s_t tmp{ 1 }; tmp <= value; tmp *= 2) {
        ++ret;
    }
    return ret;
}

void QueueR::delete_(const Node* node) {
    if (node->left != nullptr) {
        delete_(node->left);
    }
    if (node->right != nullptr) {
        delete_(node->right);
    }
    delete node;
}

QueueR::Node* QueueR::copy_(const Node* node, const Node* top, const Node* last) {
    Node* copy = new Node{ node->value, nullptr, nullptr, const_cast<Node*>(top) };
    if (node->left != nullptr) {
        copy->left = copy_(node->left, copy, last);
    }
    if (node->right != nullptr) {
        copy->right = copy_(node->right, copy, last);
    }
    if (node == last) {
        last_ = copy;
    }
    return copy;
}
