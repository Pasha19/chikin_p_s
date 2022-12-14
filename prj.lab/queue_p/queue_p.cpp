#include <queue_p/queue_p.hpp>

#include <algorithm>
#include <cassert>
#include <memory>
#include <vector>

QueueP::QueueP(const std::vector<el_t>& data)
    : root_()
    , last_()
    , size_(0)
{
    for (const auto el : data) {
        push(el);
    }
}

QueueP::QueueP(const QueueP& queueR)
    : root_()
    , last_()
    , size_(queueR.size_)
{
    root_ = copy_(queueR.root_, nullptr, queueR.last_);
}

QueueP::QueueP(QueueP&& queueR) noexcept
    : root_(std::move(queueR.root_))
    , last_(queueR.last_)
    , size_(queueR.size_)
{
    queueR.root_ = nullptr;
    queueR.last_ = nullptr;
    queueR.size_ = 0;
}

QueueP& QueueP::operator=(const QueueP& queueR) {
    if (&queueR == this) {
        return *this;
    }
    size_ = queueR.size_;
    root_ = copy_(queueR.root_, nullptr, queueR.last_);
    return *this;
}

QueueP& QueueP::operator=(QueueP&& queueR) noexcept {
    if (&queueR == this) {
        return *this;
    }
    root_ = std::move(queueR.root_);
    last_ = queueR.last_;
    size_ = queueR.size_;
    queueR.root_ = nullptr;
    queueR.last_ = nullptr;
    queueR.size_ = 0;
    return *this;
}

const QueueP::el_t& QueueP::top() const {
    assert(root_ != nullptr && size_ != 0);
    return root_->value;
}

void QueueP::pop() noexcept {
    assert(root_ != nullptr && size_ != 0);
    if (size_ == 1) {
        assert(last_ == root_.get() && root_->left == nullptr && root_->right == nullptr);
        root_ = nullptr;
        last_ = nullptr;
        size_ = 0;
        return;
    }
    if (size_ == 2) {
        assert(root_->left != nullptr && root_->right == nullptr);
        root_->value = root_->left->value;
        root_->left = nullptr;
        last_ = root_.get();
        size_ = 1;
        return;
    }
    remove_();
    siftDown_();
}

void QueueP::push(const el_t& value) {
    if (size_ == 0) {
        root_ = std::make_unique<Node>(value); 
        last_ = root_.get();
        ++size_;
        return;
    }
    append_(value);
    siftUp_();
}

void QueueP::append_(const el_t value) {
    const s_t newSize{ size_ + 1 };
    const s_t level{ log2_(newSize) };
    s_t rowSize{ 1 << level };
    s_t rowMid{ rowSize / 2 };
    s_t rowIndex{ newSize - rowSize };
    Node* node{ root_.get() };
    for (s_t i{ 1 }; i < level; ++i) {
        if (rowIndex < rowMid) {
            node = node->left.get();
        } else {
            node = node->right.get();
            rowIndex -= rowMid;
        }
        assert(node != nullptr);
        rowSize /= 2;
        rowMid = rowSize / 2;
    }
    assert(rowSize == 2 && rowIndex < 2);
    std::unique_ptr<Node> newNode{ std::make_unique<Node>(value, nullptr, nullptr, node) };
    last_ = newNode.get();
    (rowIndex == 0 ? node->left : node->right) = std::move(newNode);
    size_ = newSize;
}

void QueueP::remove_() {
    assert(size_ > 1);
    root_->value = last_->value;
    Node* lastTop = const_cast<Node*>(last_->top);
    assert(lastTop);
    (lastTop->left.get() == last_ ? lastTop->left : lastTop->right) = nullptr;
    last_ = nullptr;
    --size_;
    const s_t level{ log2_(size_) };
    s_t rowSize{ 1 << level };
    s_t rowMid{ rowSize / 2 };
    s_t rowIndex{ size_ - rowSize };
    Node* node{ root_.get() };
    for (s_t i{ 0 }; i < level; ++i) {
        if (rowIndex < rowMid) {
            node = node->left.get();
        } else {
            node = node->right.get();
            rowIndex -= rowMid;
        }
        assert(node != nullptr);
        rowSize /= 2;
        rowMid = rowSize / 2;
    }
    last_ = node;
}

void QueueP::siftUp_() {
    Node* node{ last_ };
    while (Node* nodeTop = const_cast<Node*>(node->top)) {
        if (node->value >= nodeTop->value) {
            return;
        }
        std::swap(node->value, nodeTop->value);
        node = nodeTop;
    }
}

void QueueP::siftDown_() {
    assert(size_ > 1);
    Node* node{ root_.get() };
    while (node != nullptr) {
        Node* const left{ node->left.get() };
        Node* const right{ node->right.get() };
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
        Node* const min{ left->value < right->value ? left : right };
        if (min->value >= node->value) {
            return;
        }
        std::swap(min->value, node->value);
        node = min;
    }
    assert(false);
}

QueueP::s_t QueueP::log2_(const s_t value) {
    assert(value > 0);
    s_t ret{ -1 };
    for (s_t tmp{ 1 }; tmp <= value; tmp *= 2) {
        ++ret;
    }
    return ret;
}

std::unique_ptr<QueueP::Node> QueueP::copy_(
    const std::unique_ptr<Node>& node,
    Node* const top,
    const Node* const last
) {
    std::unique_ptr<Node> copy{ std::make_unique<Node>(node->value, nullptr, nullptr, top) };
    if (node->left != nullptr) {
        copy->left = copy_(node->left, copy.get(), last);
    }
    if (node->right != nullptr) {
        copy->right = copy_(node->right, copy.get(), last);
    }
    if (node.get() == last) {
        last_ = copy.get();
    }
    return copy;
}
