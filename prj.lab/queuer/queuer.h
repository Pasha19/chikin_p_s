#ifndef QUEUER_H_
#define QUEUER_H_

class QueueR {
public:
    using el_t = int;
    using s_t = int;

    QueueR() = default;
    QueueR(const QueueR& queueR);
    QueueR(QueueR&& queueR);
    ~QueueR();

    QueueR& operator=(const QueueR& queueR);
    QueueR& operator=(QueueR&& queueR);

    s_t size() const { return size_; }
    el_t top() const;
    void pop();
    void push(const el_t value);

private:
    struct Node {
        el_t value;
        Node* left;
        Node* right;
        Node* top;
    };

    void append_(const el_t value);
    void remove_();
    void sinkUp_();
    void sinkDown_();
    static s_t log2_(const s_t value);

    static void delete_(const Node* node);
    Node* copy_(const Node* node, const Node* top, const Node* last);

    Node* root_{ nullptr };
    Node* last_{ nullptr };
    s_t size_{ 0 };
};

#endif
