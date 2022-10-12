#include <clib/queue.h>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <iostream>
#include <vector>

TEST_SUITE_BEGIN("Clib");

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
        Queue* qp{ queue_new() };
        int size{ 0 };
        REQUIRE_EQ(queue_size(qp), size);
        int min{ numbers[0] };
        for (const int num: numbers) {
            ++size;
            queue_push(qp, num);
            if (num < min) {
                min = num;
            }
            REQUIRE_EQ(queue_size(qp), size);
            REQUIRE_EQ(queue_top(qp), min);
        }
        std::sort(numbers.begin(), numbers.end());
        for (const int num: numbers) {
            REQUIRE_EQ(queue_top(qp), num);
            queue_pop(qp);
            --size;
            REQUIRE_EQ(queue_size(qp), size);
        }
        queue_delete(qp);
        qp = nullptr;
    }
}

TEST_CASE("copy") {
    Queue* qp1{ queue_new() };
    std::vector<int> numbers{ 1, 10, 5, 4, 7, 1 };
    for (const int num: numbers) {
        queue_push(qp1, num);
    }
    Queue* qp2{ queue_copy(qp1) };
    REQUIRE_EQ(queue_size(qp1), queue_size(qp2));
    std::sort(numbers.begin(), numbers.end());
    for (decltype(numbers.size()) i{0}; i < numbers.size(); ++i) {
        REQUIRE_EQ(queue_top(qp1), queue_top(qp2));
        queue_pop(qp1);
        queue_pop(qp2);
    }
    REQUIRE_EQ(queue_size(qp1), 0);
    REQUIRE_EQ(queue_size(qp2), 0);
    queue_delete( qp1 );
    queue_delete( qp2 );
}

TEST_CASE("assign") {
    Queue* qp1{ queue_new() };
    std::vector<int> numbers{ 1, 10, 5, 4, 7, 1 };
    for (const int num: numbers) {
        queue_push(qp1, num);
    }
    Queue* qp2{ queue_new() };
    queue_push(qp2, 2);
    queue_push(qp2, 3);
    queue_assign(qp2, qp1);
    REQUIRE_EQ(queue_size(qp1), queue_size(qp2));
    std::sort(numbers.begin(), numbers.end());
    for (decltype(numbers.size()) i{ 0 }; i < numbers.size(); ++i) {
        REQUIRE_EQ(queue_top(qp1), queue_top(qp2));
        queue_pop(qp1);
        queue_pop(qp2);
    }
    REQUIRE_EQ(queue_size(qp1), 0);
    REQUIRE_EQ(queue_size(qp2), 0);
    queue_delete(qp1);
    queue_delete(qp2);
}

TEST_CASE("self_assign") {
    Queue* qp{ queue_new() };
    queue_push(qp, 10);
    queue_assign(qp, qp);
    REQUIRE_EQ(queue_size(qp), 1);
    queue_delete(qp);
}

TEST_CASE("move") {
    Queue* qp1{ queue_new() };
    std::vector<int> numbers{ 1, 10, 5, 4, 7, 1 };
    for (const int num: numbers) {
        queue_push(qp1, num);
    }
    Queue* qp2{ queue_move(qp1) };
    REQUIRE_EQ(queue_size(qp1), 0);
    REQUIRE_EQ(queue_size(qp2), numbers.size());
    std::sort(numbers.begin(), numbers.end());
    for (const int num: numbers) {
        REQUIRE_EQ(queue_top(qp2), num);
        queue_pop(qp2);
    }
    REQUIRE_EQ(queue_size(qp2), 0);
    queue_delete(qp1);
    queue_delete(qp2);
}

TEST_CASE("move_assign") {
    Queue* qp1{ queue_new() };
    std::vector<int> numbers{ 1, 10, 5, 4, 7, 1 };
    for (const int num: numbers) {
        queue_push(qp1, num);
    }
    Queue* qp2{ queue_new() };
    queue_push(qp2, 2);
    queue_push(qp2, 3);
    queue_move_assign(qp2, qp1);
    REQUIRE_EQ(queue_size(qp1), 0);
    REQUIRE_EQ(queue_size(qp2), numbers.size());
    std::sort(numbers.begin(), numbers.end());
    for (const int num: numbers) {
        REQUIRE_EQ(queue_top(qp2), num);
        queue_pop(qp2);
    }
    REQUIRE_EQ(queue_size(qp2), 0);
    queue_delete(qp1);
    queue_delete(qp2);
}

TEST_CASE("move_self_assign") {
    Queue* qp{ queue_new() };
    queue_push(qp, 10);
    queue_move_assign(qp, qp);
    REQUIRE_EQ(queue_size(qp), 1);
    queue_delete(qp);
}

TEST_CASE("memory") {
    Queue* qp{ queue_new() };
    for (int i{0}; i < 100; ++i) {
        queue_push(qp, i);
    }
    REQUIRE_EQ(queue_size(qp), 100);
    queue_delete(qp);
}

TEST_SUITE_END();
