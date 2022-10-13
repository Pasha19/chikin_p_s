#include <queue_r/queue_r.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <vector>

TEST_SUITE_BEGIN("QueueR");

TEST_CASE("construct_from_vector") {
    std::vector<int> data{ 1, 2, 10, 1, 2 };
    QueueR qp{ data };
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
        QueueR qr{};
        int size{ 0 };
        REQUIRE_EQ(qr.isEmpty(), size == 0);
        int min{ numbers[0] };
        for (const int num: numbers) {
            ++size;
            qr.push(num);
            if (num < min) {
                min = num;
            }
            REQUIRE_EQ(qr.isEmpty(), size == 0);
            REQUIRE_EQ(qr.top(), min);
        }
        std::sort(numbers.begin(), numbers.end());
        for (const int num: numbers) {
            REQUIRE_EQ(qr.top(), num);
            qr.pop();
            --size;
            REQUIRE_EQ(qr.isEmpty(), size == 0);
        }
    }
}

TEST_CASE("copy") {
    QueueR qr1{};
    std::vector<int> numbers{ 1, 10, 5, 4, 7, 1 };
    for (const int num: numbers) {
        qr1.push(num);
    }
    QueueR qr2{ qr1 };
    std::sort(numbers.begin(), numbers.end());
    for (decltype(numbers.size()) i{ 0 }; i < numbers.size(); ++i) {
        REQUIRE_EQ(qr1.top(), qr2.top());
        qr1.pop();
        qr2.pop();
    }
    REQUIRE(qr1.isEmpty());
    REQUIRE(qr2.isEmpty());
}

TEST_CASE("assign") {
    QueueR qr1{};
    std::vector<int> numbers{ 1, 10, 5, 4, 7, 1 };
    for (const int num: numbers) {
        qr1.push(num);
    }
    QueueR qr2{};
    qr2.push(2);
    qr2.push(3);
    qr2 = qr1;
    std::sort(numbers.begin(), numbers.end());
    for (decltype(numbers.size()) i{0}; i < numbers.size(); ++i) {
        REQUIRE_EQ(qr1.top(), qr2.top());
        qr1.pop();
        qr2.pop();
    }
    REQUIRE(qr1.isEmpty());
    REQUIRE(qr2.isEmpty());
}

TEST_CASE("self_assign") {
    QueueR qr{};
    qr.push(10);
    qr = qr;
    REQUIRE_FALSE(qr.isEmpty());
}

TEST_CASE("move") {
    QueueR qr1{};
    std::vector<int> numbers{ 1, 10, 5, 4, 7, 1 };
    for (const int num: numbers) {
        qr1.push(num);
    }
    QueueR qr2{ std::move(qr1) };
    REQUIRE(qr1.isEmpty());
    REQUIRE_FALSE(qr2.isEmpty());
    std::sort(numbers.begin(), numbers.end());
    for (const int num: numbers) {
        REQUIRE_EQ(qr2.top(), num);
        qr2.pop();
    }
    REQUIRE(qr2.isEmpty());
}

TEST_CASE("move_assign") {
    QueueR qr1{};
    std::vector<int> numbers{ 1, 10, 5, 4, 7, 1 };
    for (const int num: numbers) {
        qr1.push(num);
    }
    QueueR qr2{};
    qr2.push(2);
    qr2.push(3);
    qr2 = std::move(qr1);
    REQUIRE(qr1.isEmpty());
    REQUIRE_FALSE(qr2.isEmpty());
    std::sort(numbers.begin(), numbers.end());
    for (const int num: numbers) {
        REQUIRE_EQ(qr2.top(), num);
        qr2.pop();
    }
    REQUIRE(qr2.isEmpty());
}

TEST_CASE("move_self_assign") {
    QueueR qr{};
    qr.push(10);
    qr = std::move(qr);
    REQUIRE_FALSE(qr.isEmpty());

}

TEST_CASE("memory") {
    QueueR qr{};
    for (int i{ 0 }; i < 100; ++i) {
        qr.push(i);
    }
    REQUIRE_FALSE(qr.isEmpty());
}

TEST_SUITE_END();
