// ReSharper disable CppUseAuto
#include "unique_ptr.hpp"

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include <cstdint>

template <typename T>
using unique_ptr = my::unique_ptr<T>;
// Alternative for all possible cases:
// using namespace my;

unique_ptr<int> mock_ptr;

TEST_CASE("Create unique ptr", "[unique_ptr]") {
    SECTION("Integer") {
        SECTION("Empty unique ptr") {
            const unique_ptr<int> empty_ptr;
            CHECK(empty_ptr.get() == nullptr);
        }

        SECTION("Nullptr unique ptr") {
            const unique_ptr<int> null_ptr{nullptr};
            CHECK(null_ptr.get() == nullptr);
        }

        SECTION("Raw data unique ptr") {
            const unique_ptr init_ptr{new int(1)};
            CHECK(init_ptr.get() != nullptr);

            int* raw_ptr = new int(1);
            const unique_ptr ptr{raw_ptr};
            CHECK(ptr.get() == raw_ptr);
        }
    }

    SECTION("Double empty unique ptr") {
        const unique_ptr<double> empty_ptr;
        CHECK(empty_ptr.get() == nullptr);
    }
}

TEST_CASE("Destroy unique ptr", "[unique_ptr]") {
    SKIP("Checking releasing memory of pointer required custom deleter or system signal handling");

    GIVEN("Integer unique ptr") {
        int* raw_ptr = new int(1);
        WHEN("Releasing memory of raw_ptr by creating and destroying unique ptr") {
            {
                const unique_ptr ptr{raw_ptr};
                REQUIRE(ptr.get() == raw_ptr);
            }
            THEN("Releasing memory of raw_ptr, once again") {
                REQUIRE_THROWS(delete raw_ptr);
            }
        }
    }
}

TEST_CASE("Copy unique ptr", "[unique_ptr]") {
    SECTION("Copy constructor should be deleted") {
        CHECK(!std::is_copy_constructible_v<unique_ptr<int>>);
    }

    SECTION("Copy assignment should be deleted") {
        CHECK(!std::is_copy_assignable_v<unique_ptr<int>>);
    }
}

TEST_CASE("Move unique ptr", "[unique_ptr]") {
    constexpr int assignedValue = 10;

    SECTION("Move constructor") {
        REQUIRE(std::is_move_constructible_v<unique_ptr<int>>);

        unique_ptr ptr{new int(1)};
        *ptr = assignedValue;
        const unique_ptr ptr2{std::move(ptr)};

        CHECK(ptr.get() == nullptr);
        CHECK(*ptr2 == assignedValue);
    }

    SECTION("Move assignment") {
        REQUIRE(std::is_move_assignable_v<unique_ptr<int>>);

        unique_ptr ptr{new int(1)};
        *ptr = assignedValue;
        unique_ptr ptr2{new int(1)};
        ptr2 = std::move(ptr);
        ptr2 = std::move(ptr2);

        CHECK(ptr.get() == nullptr);
        CHECK(*ptr2.get() == assignedValue);
    }
}

struct ValueTest {
    int value;

    [[nodiscard]] int get_value() const noexcept {
        return value;
    }

    void set_value(const int new_value) noexcept {
        this->value = new_value;
    }
};

TEST_CASE("Member functions", "[unique_ptr]") {
    SECTION("operator*()") {
        const unique_ptr ptr{new int(1)};
        *ptr = 10;
        REQUIRE(*ptr == 10);
        *ptr = 20;
        REQUIRE(*ptr == 20);
    }

    SECTION("operator->") {
        const unique_ptr ptr{new ValueTest{10}};
        ptr->set_value(20);
        REQUIRE(ptr->get_value() == 20);
        ptr->set_value(-30);
        REQUIRE(ptr->get_value() == -30);
    }

    SECTION("get()") {
        const unique_ptr ptr{new ValueTest{10}};
        ValueTest* value_test = ptr.get();
        REQUIRE(value_test->value == 10);
    }

    SECTION("release()") {
        unique_ptr ptr{new ValueTest{10}};
        ValueTest* value_ptr = ptr.release();
        delete value_ptr;
        REQUIRE(ptr.get() == nullptr);
    }

    SECTION("reset()") {
        unique_ptr ptr{new ValueTest{10}};
        ValueTest* new_value = new ValueTest{20};
        ptr.reset(nullptr);
        ptr.reset(nullptr);
        ptr.reset(nullptr);
        ptr.reset(new_value);
        ptr.reset(new_value);
        ptr.reset(new_value);
        REQUIRE(ptr.get() == new_value);
    }
}
