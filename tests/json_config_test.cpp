#include <catch2/catch_test_macros.hpp>

#include "msh/utils/JsonConfig.hpp"
#include "msh/utils/file_io.hpp"
#include "nlohmann/json.hpp"

using namespace msh::utils;

enum class MyEnum { Value1, Value2, Default };
NLOHMANN_JSON_SERIALIZE_ENUM(MyEnum, {
                                         {MyEnum::Default, "default"},
                                         {MyEnum::Value1, "value1"},
                                         {MyEnum::Value2, "value2"},
                                     })

TEST_CASE("get value from json", "[json_config]") {
    SECTION("int32_t") {
        nlohmann::json json_data = {{"int32_t", 42}, {"wrong_type", "value2"}};
        int32_t value = JsonConfig::getSafe(json_data, "int32_t", int32_t{});
        REQUIRE(value == 42);
        int32_t wrong_type_value = JsonConfig::getSafe(json_data, "wrong_type", int32_t{});
        REQUIRE(wrong_type_value == 0);
        int32_t missing_value = JsonConfig::getSafe(json_data, "missing_key", int32_t{});
        REQUIRE(missing_value == 0);
    }

    SECTION("bool") {
        nlohmann::json json_data = {{"bool", true}, {"wrong_type", "value2"}};
        bool value = JsonConfig::getSafe(json_data, "bool", bool{});
        REQUIRE(value == true);
        bool wrong_type_value = JsonConfig::getSafe(json_data, "wrong_type", bool{});
        REQUIRE(wrong_type_value == false);
        bool missing_value = JsonConfig::getSafe(json_data, "missing_key", bool{});
        REQUIRE(missing_value == false);
    }

    SECTION("float") {
        nlohmann::json json_data = {{"float", 3.14f}, {"wrong_type", "value2"}, {"wrong_type2", 12}};
        float value = JsonConfig::getSafe(json_data, "float", float{});
        REQUIRE(value == 3.14f);
        float wrong_type_value = JsonConfig::getSafe(json_data, "wrong_type", float{});
        REQUIRE(wrong_type_value == 0.0f);
        float wrong_type2_value = JsonConfig::getSafe(json_data, "wrong_type2", float{});
        REQUIRE(wrong_type2_value == 0.0f);
        float missing_value = JsonConfig::getSafe(json_data, "missing_key", float{});
        REQUIRE(missing_value == 0.0f);
    }

    SECTION("double") {
        nlohmann::json json_data = {{"double", 3.14}, {"wrong_type", "value2"}};
        double value = JsonConfig::getSafe(json_data, "double", double{});
        REQUIRE(value == 3.14);
        double wrong_type_value = JsonConfig::getSafe(json_data, "wrong_type", double{});
        REQUIRE(wrong_type_value == 0.0);
        double missing_value = JsonConfig::getSafe(json_data, "missing_key", double{});
        REQUIRE(missing_value == 0.0);
    }

    SECTION("string") {
        nlohmann::json json_data = {{"string", "value1"}, {"wrong_type", 42}};
        std::string value = JsonConfig::getSafe(json_data, "string", std::string{});
        REQUIRE(value == "value1");
        std::string wrong_type_value = JsonConfig::getSafe(json_data, "wrong_type", std::string{});
        REQUIRE(wrong_type_value == "");
        std::string missing_value = JsonConfig::getSafe(json_data, "missing_key", std::string{});
        REQUIRE(missing_value == "");
    }

    SECTION("enum") {
        nlohmann::json json_data = {{"enum", "value1"}, {"wrong_type", 42}};
        MyEnum value = JsonConfig::getSafe(json_data, "enum", MyEnum::Default);
        REQUIRE(value == MyEnum::Value1);
        MyEnum wrong_type_value = JsonConfig::getSafe(json_data, "wrong_type", MyEnum::Default);
        REQUIRE(wrong_type_value == MyEnum::Default);
        MyEnum missing_value = JsonConfig::getSafe(json_data, "missing_key", MyEnum::Default);
        REQUIRE(missing_value == MyEnum::Default);
    }

    SECTION("uint8_t") {
        nlohmann::json json_data = {{"uint8_t", 42}, {"wrong_type", -12}, {"wrong_type2", 300}};
        uint8_t value = JsonConfig::getSafe(json_data, "uint8_t", uint8_t{});
        REQUIRE(value == 42);
        uint8_t wrong_type_value = JsonConfig::getSafe(json_data, "wrong_type", uint8_t{});
        REQUIRE(wrong_type_value == 0);
        uint8_t wrong_type2_value = JsonConfig::getSafe(json_data, "wrong_type2", uint8_t{});
        REQUIRE(wrong_type2_value == 0);
        uint8_t missing_value = JsonConfig::getSafe(json_data, "missing_key", uint8_t{});
        REQUIRE(missing_value == 0);
    }

    SECTION("int8_t") {
        nlohmann::json json_data = {{"int8_t", -12}, {"wrong_type", 300}, {"wrong_type2", -400}};
        int8_t value = JsonConfig::getSafe(json_data, "int8_t", int8_t{});
        REQUIRE(value == -12);
        int8_t wrong_type_value = JsonConfig::getSafe(json_data, "wrong_type", int8_t{});
        REQUIRE(wrong_type_value == 0);
        int8_t wrong_type2_value = JsonConfig::getSafe(json_data, "wrong_type2", int8_t{});
        REQUIRE(wrong_type2_value == 0);
        int8_t missing_value = JsonConfig::getSafe(json_data, "missing_key", int8_t{});
        REQUIRE(missing_value == 0);
    }
}