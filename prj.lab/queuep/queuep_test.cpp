#include <queuep/queuep.h>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <iostream>
#include <vector>

TEST_SUITE_BEGIN("QueueP");

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
        CAPTURE(numbers);
        QueueP qp{};
        int size{ 0 };
        REQUIRE_EQ(qp.size(), size);
        int min{ numbers[0] };
        for (const int num: numbers) {
            ++size;
            qp.push(num);
            if (num < min) {
                min = num;
            }
            REQUIRE_EQ(qp.size(), size);
            REQUIRE_EQ(qp.top(), min);
        }
        std::sort(numbers.begin(), numbers.end());
        for (const int num: numbers) {
            REQUIRE_EQ(qp.top(), num);
            qp.pop();
            --size;
            REQUIRE_EQ(qp.size(), size);
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
    REQUIRE_EQ(qp1.size(), qp2.size());
    std::sort(numbers.begin(), numbers.end());
    for (decltype(numbers.size()) i{0}; i < numbers.size(); ++i) {
        REQUIRE_EQ(qp1.top(), qp2.top());
        qp1.pop();
        qp2.pop();
    }
    REQUIRE_EQ(qp1.size(), 0);
    REQUIRE_EQ(qp2.size(), 0);
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
    REQUIRE_EQ(qp1.size(), qp2.size());
    std::sort(numbers.begin(), numbers.end());
    for (decltype(numbers.size()) i{0}; i < numbers.size(); ++i) {
        REQUIRE_EQ(qp1.top(), qp2.top());
        qp1.pop();
        qp2.pop();
    }
    REQUIRE_EQ(qp1.size(), 0);
    REQUIRE_EQ(qp2.size(), 0);
}

TEST_CASE("self_assign") {
    QueueP qp{};
    qp.push(10);
    qp = qp;
    REQUIRE_EQ(qp.size(), 1);
}

TEST_CASE("move") {
    QueueP qp1{};
    std::vector<int> numbers{ 1, 10, 5, 4, 7, 1 };
    for (const int num: numbers) {
        qp1.push(num);
    }
    QueueP qp2{ std::move(qp1) };
    REQUIRE_EQ(qp1.size(), 0);
    REQUIRE_EQ(qp2.size(), numbers.size());
    std::sort(numbers.begin(), numbers.end());
    for (const int num: numbers) {
        REQUIRE_EQ(qp2.top(), num);
        qp2.pop();
    }
    REQUIRE_EQ(qp2.size(), 0);
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
    REQUIRE_EQ(qp1.size(), 0);
    REQUIRE_EQ(qp2.size(), numbers.size());
    std::sort(numbers.begin(), numbers.end());
    for (const int num: numbers) {
        REQUIRE_EQ(qp2.top(), num);
        qp2.pop();
    }
    REQUIRE_EQ(qp2.size(), 0);
}

TEST_CASE("move_self_assign") {
    QueueP qp{};
    qp.push(10);
    qp = std::move(qp);
    REQUIRE_EQ(qp.size(), 1);

}

TEST_CASE("memory") {
    QueueP qp{};
    for (int i{0}; i < 100; ++i) {
        qp.push(i);
    }
    REQUIRE_EQ(qp.size(), 100);
}

TEST_SUITE_END();
