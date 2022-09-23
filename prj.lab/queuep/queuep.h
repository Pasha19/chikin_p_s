#ifndef QUEUEP_H_
#define QUEUEP_H_

#include <memory>

class QueueP {
public:
    using el_t = int;
    using s_t = int;

    QueueP() = default;
    QueueP(const QueueP& queueR);
    QueueP(QueueP&& queueR);
    ~QueueP() = default;

    QueueP& operator=(const QueueP& queueR);
    QueueP& operator=(QueueP&& queueR);

    s_t size() const { return size_; }
    el_t top() const;
    void pop();
    void push(const el_t value);

private:
    struct Node {
        Node(
            const el_t value_ = el_t{},
            const std::shared_ptr<Node>& left_ = nullptr,
            const std::shared_ptr<Node>& right_ = nullptr,
            const std::shared_ptr<Node>& top_ = nullptr
        )
            : value(value_)
            , left(left_)
            , right(right_)
            , top(top_)
        {}
        el_t value;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        std::shared_ptr<Node> top;
    };

    void append_(const el_t value);
    void remove_();
    void sinkUp_();
    void sinkDown_();
    static s_t log2_(const s_t value);

    std::shared_ptr<Node> copy_(
        const std::shared_ptr<Node>& node,
        const std::shared_ptr<Node>& top,
        const std::shared_ptr<Node>& last
    );

    std::shared_ptr<Node> root_;
    std::shared_ptr<Node> last_;
    s_t size_{ 0 };
};

#endif
