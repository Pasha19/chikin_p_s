#ifndef QUEUEP_H_
#define QUEUEP_H_

#include <memory>

class QueueP {
public:
    using el_t = int;
    using s_t = int;

    QueueP() = default;
    QueueP(const QueueP& queueR);
    QueueP(QueueP&& queueR) noexcept;
    ~QueueP() = default;

    QueueP& operator=(const QueueP& queueR);
    QueueP& operator=(QueueP&& queueR) noexcept;

    s_t size() const { return size_; }
    el_t top() const;
    void pop();
    void push(el_t value);

private:
    struct Node {
        explicit Node(
            el_t value_ = el_t{},
            std::unique_ptr<Node>&& left_ = nullptr,
            std::unique_ptr<Node>&& right_ = nullptr,
            Node* const top_ = nullptr
        )
            : value(value_)
            , left(std::move(left_))
            , right(std::move(right_))
            , top(top_)
        {}
        Node(const Node&) = delete;
        Node(Node&&) noexcept = default;
        ~Node() = default;
        Node& operator=(const Node&) = delete;
        Node& operator=(Node&&) = delete;
        el_t value;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
        const Node* const top{ nullptr };
    };

    void append_(el_t value);
    void remove_();
    void siftUp_();
    void siftDown_();
    static s_t log2_(s_t value);

    std::unique_ptr<Node> copy_(
        const std::unique_ptr<Node>& node,
        Node* top,
        const Node* last
    );

    std::unique_ptr<Node> root_;
    Node* last_{ nullptr };
    s_t size_{ 0 };
};

#endif
