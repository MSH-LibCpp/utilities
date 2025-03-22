#include <catch2/catch_test_macros.hpp>
#include <msh/utils/byte_array.hpp>
#include <string>

using namespace msh::utils;

TEST_CASE("ByteArray basic operations", "[ByteArray]") {
    SECTION("Default constructor") {
        ByteArray arr;
        CHECK(arr.empty());
        CHECK(arr.size() == 0);
    }

    SECTION("Size value constructor") {
        ByteArray arr(5, 0xFF);
        CHECK(arr.size() == 5);
        for (size_t i = 0; i < arr.size(); ++i) {
            CHECK(arr[i] == 0xFF);
        }
    }

    SECTION("Raw data constructor") {
        uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
        ByteArray arr(data, sizeof(data));
        CHECK(arr.size() == 4);
        for (size_t i = 0; i < arr.size(); ++i) {
            CHECK(arr[i] == data[i]);
        }
    }

    SECTION("Initializer list constructor") {
        ByteArray arr{0x01, 0x02, 0x03, 0x04};
        CHECK(arr.size() == 4);
        CHECK(arr[0] == 0x01);
        CHECK(arr[1] == 0x02);
        CHECK(arr[2] == 0x03);
        CHECK(arr[3] == 0x04);
    }

    SECTION("String constructor") {
        std::string str = "Hello";
        ByteArray arr(str);
        CHECK(arr.size() == str.size());
        for (size_t i = 0; i < str.size(); ++i) {
            CHECK(arr[i] == static_cast<uint8_t>(str[i]));
        }
    }
}

TEST_CASE("ByteArray copy and move operations", "[ByteArray]") {
    SECTION("Copy constructor") {
        ByteArray original{0x01, 0x02, 0x03};
        ByteArray copy(original);
        CHECK(copy.size() == original.size());
        for (size_t i = 0; i < original.size(); ++i) {
            CHECK(copy[i] == original[i]);
        }
    }

    SECTION("Move constructor") {
        ByteArray original{0x01, 0x02, 0x03};
        size_t originalSize = original.size();
        ByteArray moved(std::move(original));
        CHECK(moved.size() == originalSize);
        CHECK(original.empty());  // Testing moved-from state
    }
}

TEST_CASE("ByteArray element access", "[ByteArray]") {
    ByteArray arr{0x01, 0x02, 0x03};

    SECTION("Operator[] access") {
        CHECK(arr[1] == 0x02);
    }

    SECTION("at() method") {
        CHECK(arr.at(1) == 0x02);
        CHECK_THROWS_AS(arr.at(3), std::out_of_range);
    }
}

TEST_CASE("ByteArray modifiers", "[ByteArray]") {
    SECTION("Append ByteArray") {
        ByteArray arr1{0x01, 0x02};
        ByteArray arr2{0x03, 0x04};
        arr1.append(arr2);
        CHECK(arr1.size() == 4);
        CHECK(arr1[2] == 0x03);
        CHECK(arr1[3] == 0x04);
    }

    SECTION("Append raw data") {
        ByteArray arr{0x01, 0x02};
        uint8_t data[] = {0x03, 0x04};
        arr.append(data, sizeof(data));
        CHECK(arr.size() == 4);
        CHECK(arr[2] == 0x03);
        CHECK(arr[3] == 0x04);
    }

    SECTION("Resize") {
        ByteArray arr{0x01, 0x02};
        arr.resize(4, 0xFF);
        CHECK(arr.size() == 4);
        CHECK(arr[0] == 0x01);
        CHECK(arr[1] == 0x02);
        CHECK(arr[2] == 0xFF);
        CHECK(arr[3] == 0xFF);
    }

    SECTION("Clear") {
        ByteArray arr{0x01, 0x02};
        arr.clear();
        CHECK(arr.empty());
        CHECK(arr.size() == 0);
    }

    SECTION("Reserve") {
        ByteArray arr;
        arr.reserve(100);
        CHECK(arr.capacity() >= 100);
        CHECK(arr.size() == 0);
    }
}

TEST_CASE("ByteArray comparison operators", "[ByteArray]") {
    ByteArray arr1{0x01, 0x02};
    ByteArray arr2{0x01, 0x02};
    ByteArray arr3{0x01, 0x03};

    SECTION("Equality") {
        CHECK(arr1 == arr2);
    }

    SECTION("Inequality") {
        CHECK(arr1 != arr3);
    }
}

TEST_CASE("ByteArray string operations", "[ByteArray]") {
    SECTION("toHexString") {
        ByteArray arr{0x01, 0xAB, 0xFF};
        CHECK(arr.toHexString() == "01ABFF");
    }

    SECTION("fromHexString") {
        ByteArray arr = ByteArray::fromHexString("01ABFF");
        CHECK(arr.size() == 3);
        CHECK(arr[0] == 0x01);
        CHECK(arr[1] == 0xAB);
        CHECK(arr[2] == 0xFF);
        
        CHECK_THROWS_AS(ByteArray::fromHexString("0"), std::invalid_argument);
        CHECK_THROWS_AS(ByteArray::fromHexString("0G"), std::invalid_argument);
    }

    SECTION("string conversion") {
        std::string original = "Hello";
        ByteArray arr(original);
        CHECK(arr.string() == original);
    }
}

TEST_CASE("ByteArray iterator operations", "[ByteArray]") {
    ByteArray arr{0x01, 0x02, 0x03};
    std::vector<uint8_t> vec;

    SECTION("Range-based for loop") {
        for (const auto& byte : arr) {
            vec.push_back(byte);
        }
        CHECK(vec.size() == 3);
        CHECK(vec[0] == 0x01);
        CHECK(vec[1] == 0x02);
        CHECK(vec[2] == 0x03);
    }
} 