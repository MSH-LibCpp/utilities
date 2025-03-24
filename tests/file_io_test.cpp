#include "msh/utils/file_io.hpp"

#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <fstream>
#include <random>

using namespace msh::utils;

TEST_CASE("file_io: read and write operations", "[file_io]") {
    // Create a temporary directory for test files
    auto temp_dir = std::filesystem::temp_directory_path() / "msh_utils_test";
    std::filesystem::create_directories(temp_dir);
    auto test_file = temp_dir / "test.bin";

    // Generate random test data
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    ByteArray test_data;
    test_data.resize(1000);
    for (auto& byte : test_data) {
        byte = static_cast<uint8_t>(dis(gen));
    }

    SECTION("write and read back data") {
        // Write test data to file
        REQUIRE(file_io::write(test_file, test_data));

        // Read back the data
        ByteArray read_data;
        REQUIRE(file_io::read(test_file, read_data));

        // Verify data matches
        REQUIRE(read_data.size() == test_data.size());
        REQUIRE(read_data == test_data);
    }

    SECTION("read non-existent file") {
        ByteArray read_data;
        REQUIRE_FALSE(file_io::read(temp_dir / "nonexistent.bin", read_data));
    }

    SECTION("write to invalid path") {
        REQUIRE_FALSE(file_io::write(temp_dir / "invalid/path/test.bin", test_data));
    }

    // Cleanup
    std::filesystem::remove_all(temp_dir);
}

TEST_CASE("file_io: large file operations", "[file_io]") {
    auto temp_dir = std::filesystem::temp_directory_path() / "msh_utils_test";
    std::filesystem::create_directories(temp_dir);
    auto test_file = temp_dir / "large_test.bin";

    // Create a large test file (10MB)
    const size_t file_size = 10 * 1024 * 1024;
    std::ofstream large_file(test_file, std::ios::binary);
    std::vector<uint8_t> buffer(4096);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    for (size_t i = 0; i < file_size; i += buffer.size()) {
        for (auto& byte : buffer) {
            byte = static_cast<uint8_t>(dis(gen));
        }
        large_file.write(reinterpret_cast<char*>(buffer.data()),
                         static_cast<std::streamsize>(std::min(buffer.size(), file_size - i)));
    }
    large_file.close();

    SECTION("read large file") {
        ByteArray read_data;
        REQUIRE(file_io::read(test_file, read_data));
        REQUIRE(read_data.size() == file_size);
    }

    // Cleanup
    std::filesystem::remove_all(temp_dir);
}