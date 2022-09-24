#include <queuep/queuep.h>

#include <algorithm>
#include <cassert>
#include <memory>

QueueP::QueueP(const QueueP& queueR)
    : root_()
    , last_()
    , size_(queueR.size_)
{
    root_ = copy_(queueR.root_, nullptr, queueR.last_);
}

QueueP::QueueP(QueueP&& queueR)
    : root_(queueR.root_)
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

QueueP& QueueP::operator=(QueueP&& queueR) {
    if (&queueR == this) {
        return *this;
    }
    root_ = queueR.root_;
    last_ = queueR.last_;
    size_ = queueR.size_;
    queueR.root_ = nullptr;
    queueR.last_ = nullptr;
    queueR.size_ = 0;
    return *this;
}

QueueP::el_t QueueP::top() const {
    assert(root_ != nullptr && size_ != 0);
    return root_->value;
}

void QueueP::pop() {
    assert(root_ != nullptr && size_ != 0);
    if (size_ == 1) {
        assert(last_ == root_ && root_->left == nullptr && root_->right == nullptr);
        root_ = nullptr;
        last_ = nullptr;
        size_ = 0;
        return;
    }
    if (size_ == 2) {
        assert(root_->left != nullptr && root_->right == nullptr);
        root_->value = root_->left->value;
        root_->left = nullptr;
        last_ = root_;
        size_ = 1;
        return;
    }
    remove_();
    sinkDown_();
}

void QueueP::push(const el_t value) {
    if (size_ == 0) {
        root_ = std::make_shared<Node>(value); 
        last_ = root_;
        ++size_;
        return;
    }
    append_(value);
    sinkUp_();
}

void QueueP::append_(const el_t value) {
    const s_t newSize{ size_ + 1 };
    const s_t level{ log2_(newSize) };
    s_t rowSize{ 1 << level };
    s_t rowMid{ rowSize / 2 };
    s_t rowIndex{ newSize - rowSize };
    std::shared_ptr<Node> node{ root_ };
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
    std::shared_ptr<Node> newNode{ std::make_shared<Node>(value, nullptr, nullptr, node) };
    (rowIndex == 0 ? node->left : node->right) = newNode;
    last_ = newNode;
    size_ = newSize;
}

void QueueP::remove_() {
    assert(size_ > 1);
    root_->value = last_->value;
    auto lastTop = last_->top.lock();
    assert(lastTop);
    (lastTop->left == last_ ? lastTop->left : lastTop->right) = nullptr;
    last_ = nullptr;
    --size_;
    const s_t level{ log2_(size_) };
    s_t rowSize{ 1 << level };
    s_t rowMid{ rowSize / 2 };
    s_t rowIndex{ size_ - rowSize };
    std::shared_ptr<Node> node{ root_ };
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

void QueueP::sinkUp_() {
    std::shared_ptr<Node> node{ last_ };
    while (auto nodeTop = node->top.lock()) {
        if (node->value >= nodeTop->value) {
            return;
        }
        std::swap(node->value, nodeTop->value);
        node = nodeTop;
    }
}

void QueueP::sinkDown_() {
    assert(size_ > 1);
    std::shared_ptr<Node> node{ root_ };
    while (node != nullptr) {
        std::shared_ptr<Node> left{ node->left };
        std::shared_ptr<Node> right{ node->right };
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
        std::shared_ptr<Node> min{ left->value < right->value ? left : right };
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

std::shared_ptr<QueueP::Node> QueueP::copy_(
    const std::shared_ptr<Node>& node,
    const std::shared_ptr<Node>& top,
    const std::shared_ptr<Node>& last
) {
    std::shared_ptr<Node> copy{ std::make_shared<Node>(node->value, nullptr, nullptr, top) };
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
