#include <queue_p/queue_p.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <vector>

TEST_SUITE_BEGIN("QueueP");

TEST_CASE("construct_from_vector") {
    std::vector<int> data{ 1, 2, 10, 1, 2 };
    QueueP qp{ data };
    std::sort(data.begin(), data.end());
    for (const auto& el : data) {
        REQUIRE_FALSE(qp.isEmpty());
        REQUIRE_EQ(qp.top(), el);
        qp.pop();
    }
    REQUIRE(qp.isEmpty());
}

TEST_CASE("push_and_pop") {
    std::vector<std::vector<int>> data{
        { 10 },
        { 10, 20 },
        { 20, 10 },
        { 10, 20, 30 },
        { 10, 30, 20 },
        { 30, 20, 10 },
        { 10, 50, 50, 30, 20, 50, 10,  5, 80, 5 },
        { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 },
        { 10, 50, 50, 30, 20, 50, 10,  5, 80, 5, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 20, 90, 75 },
    };
    for (auto& numbers: data) {
        QueueP qp{};
        int size{ 0 };
        REQUIRE_EQ(qp.isEmpty(), size == 0);
        int min{ numbers[0] };
        for (const int num: numbers) {
            ++size;
            qp.push(num);
            if (num < min) {
                min = num;
            }
            REQUIRE_EQ(qp.isEmpty(), size == 0);
            REQUIRE_EQ(qp.top(), min);
        }
        std::sort(numbers.begin(), numbers.end());
        for (const int num: numbers) {
            REQUIRE_EQ(qp.top(), num);
            qp.pop();
            --size;
            REQUIRE_EQ(qp.isEmpty(), size == 0);
        }
    }
}

TEST_CASE("copy") {
    QueueP qp1{};
    std::vector<int> numbers{ 1, 10, 5, 4, 7, 1 };
    for (const int num: numbers) {
        qp1.push(num);
    }
    QueueP qp2{ qp1 };
    std::sort(numbers.begin(), numbers.end());
    for (decltype(numbers.size()) i{ 0 }; i < numbers.size(); ++i) {
        REQUIRE_EQ(qp1.top(), qp2.top());
        qp1.pop();
        qp2.pop();
    }
    REQUIRE(qp1.isEmpty());
    REQUIRE(qp2.isEmpty());
}

TEST_CASE("assign") {
    QueueP qp1{};
    std::vector<int> numbers{ 1, 10, 5, 4, 7, 1 };
    for (const int num: numbers) {
        qp1.push(num);
    }
    QueueP qp2{};
    qp2.push(2);
    qp2.push(3);
    qp2 = qp1;
    std::sort(numbers.begin(), numbers.end());
    for (decltype(numbers.size()) i{0}; i < numbers.size(); ++i) {
        REQUIRE_EQ(qp1.top(), qp2.top());
        qp1.pop();
        qp2.pop();
    }
    REQUIRE(qp1.isEmpty());
    REQUIRE(qp2.isEmpty());
}

TEST_CASE("self_assign") {
    QueueP qp{};
    qp.push(10);
    qp = qp;
    REQUIRE_FALSE(qp.isEmpty());
}

TEST_CASE("move") {
    QueueP qp1{};
    std::vector<int> numbers{ 1, 10, 5, 4, 7, 1 };
    for (const int num: numbers) {
        qp1.push(num);
    }
    QueueP qp2{ std::move(qp1) };
    REQUIRE(qp1.isEmpty());
    REQUIRE_FALSE(qp2.isEmpty());
    std::sort(numbers.begin(), numbers.end());
    for (const int num: numbers) {
        REQUIRE_EQ(qp2.top(), num);
        qp2.pop();
    }
    REQUIRE(qp2.isEmpty());
}

TEST_CASE("move_assign") {
    QueueP qp1{};
    std::vector<int> numbers{ 1, 10, 5, 4, 7, 1 };
    for (const int num: numbers) {
        qp1.push(num);
    }
    QueueP qp2{};
    qp2.push(2);
    qp2.push(3);
    qp2 = std::move(qp1);
    REQUIRE(qp1.isEmpty());
    REQUIRE_FALSE(qp2.isEmpty());
    std::sort(numbers.begin(), numbers.end());
    for (const int num: numbers) {
        REQUIRE_EQ(qp2.top(), num);
        qp2.pop();
    }
    REQUIRE(qp2.isEmpty());
}

TEST_CASE("move_self_assign") {
    QueueP qp{};
    qp.push(10);
    qp = std::move(qp);
    REQUIRE_FALSE(qp.isEmpty());

}

TEST_CASE("memory") {
    QueueP qp{};
    for (int i{ 0 }; i < 100; ++i) {
        qp.push(i);
    }
    REQUIRE_FALSE(qp.isEmpty());
}

TEST_SUITE_END();
