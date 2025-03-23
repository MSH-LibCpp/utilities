#pragma once

#include <filesystem>
#include <fstream>
#include <plog/Log.h>

#include "byte_array.hpp"

namespace msh::utils {

namespace file_io {

/**
 * @brief Read binary data from a file into a ByteArray
 * @param path Path to the file to read
 * @param bytes ByteArray to store the read data
 * @return true if successful, false otherwise
 */
inline bool read(const std::filesystem::path& path, ByteArray& bytes) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        PLOG_ERROR << "Failed to open file for reading: " << path;
        return false;
    }

    file.seekg(0, std::ios::end);
    auto size = file.tellg();
    file.seekg(0, std::ios::beg);

    bytes.resize(static_cast<ByteArray::size_type>(size));
    file.read(reinterpret_cast<char*>(bytes.data()), size);
    
    if (!file.good()) {
        PLOG_ERROR << "Failed to read file: " << path;
        return false;
    }
    return true;
}

/**
 * @brief Write binary data from a ByteArray to a file
 * @param path Path to the file to write
 * @param bytes ByteArray containing the data to write
 * @return true if successful, false otherwise
 */
inline bool write(const std::filesystem::path& path, const ByteArray& bytes) {
    std::ofstream file(path, std::ios::binary);
    if (!file) {
        PLOG_ERROR << "Failed to open file for writing: " << path;
        return false;
    }

    file.write(reinterpret_cast<const char*>(bytes.data()),
               static_cast<std::streamsize>(bytes.size()));
    
    if (!file.good()) {
        PLOG_ERROR << "Failed to write file: " << path;
        return false;
    }
    return true;
}

}  // namespace file_io

}  // namespace msh::utils