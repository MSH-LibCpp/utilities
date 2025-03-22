#ifndef MSH_UTILS_BYTE_ARRAY_HPP
#define MSH_UTILS_BYTE_ARRAY_HPP

#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

namespace msh::utils {

class ByteArray {
  public:
    using value_type = uint8_t;
    using size_type = std::size_t;
    using iterator = typename std::vector<value_type>::iterator;
    using const_iterator = typename std::vector<value_type>::const_iterator;

    // Constructors
    ByteArray() = default;

    explicit ByteArray(size_type size, value_type value = 0) : data_(size, value) {}

    ByteArray(const value_type* data, size_type size) : data_(data, data + size) {}

    ByteArray(std::initializer_list<value_type> init) : data_(init) {}

    explicit ByteArray(const std::string& str) 
        : data_(reinterpret_cast<const value_type*>(str.data()),
                reinterpret_cast<const value_type*>(str.data()) + str.size()) {}

    // Copy operations
    ByteArray(const ByteArray&) = default;
    ByteArray& operator=(const ByteArray&) = default;

    // Move operations
    ByteArray(ByteArray&&) noexcept = default;
    ByteArray& operator=(ByteArray&&) noexcept = default;

    // Element access
    value_type& at(size_type pos) {
        return data_.at(pos);
    }
    const value_type& at(size_type pos) const {
        return data_.at(pos);
    }

    value_type& operator[](size_type pos) {
        return data_[pos];
    }
    const value_type& operator[](size_type pos) const {
        return data_[pos];
    }

    value_type* data() noexcept {
        return data_.data();
    }
    const value_type* data() const noexcept {
        return data_.data();
    }

    // Iterators
    iterator begin() noexcept {
        return data_.begin();
    }
    const_iterator begin() const noexcept {
        return data_.begin();
    }
    const_iterator cbegin() const noexcept {
        return data_.cbegin();
    }

    iterator end() noexcept {
        return data_.end();
    }
    const_iterator end() const noexcept {
        return data_.end();
    }
    const_iterator cend() const noexcept {
        return data_.cend();
    }

    // Capacity
    bool empty() const noexcept {
        return data_.empty();
    }
    size_type size() const noexcept {
        return data_.size();
    }
    void reserve(size_type new_cap) {
        data_.reserve(new_cap);
    }
    size_type capacity() const noexcept {
        return data_.capacity();
    }

    // Modifiers
    void clear() noexcept {
        data_.clear();
    }

    void append(const ByteArray& other) {
        data_.insert(data_.end(), other.begin(), other.end());
    }

    void append(const value_type* data, size_type size) {
        data_.insert(data_.end(), data, data + size);
    }

    void resize(size_type count) {
        data_.resize(count);
    }
    void resize(size_type count, value_type value) {
        data_.resize(count, value);
    }

    // Comparison operators
    bool operator==(const ByteArray& other) const {
        return data_ == other.data_;
    }

    bool operator!=(const ByteArray& other) const {
        return !(*this == other);
    }

    // Utility functions
    std::string toHexString() const {
        static const char hex_chars[] = "0123456789ABCDEF";
        std::string result;
        result.reserve(size() * 2);

        for (value_type byte : data_) {
            result.push_back(hex_chars[byte >> 4]);
            result.push_back(hex_chars[byte & 0x0F]);
        }

        return result;
    }

    std::string string() const {
        return std::string(reinterpret_cast<const char*>(data_.data()), data_.size());
    }

    static ByteArray fromHexString(const std::string& hex) {
        if (hex.length() % 2 != 0) {
            throw std::invalid_argument("Hex string length must be even");
        }

        ByteArray result;
        result.reserve(hex.length() / 2);

        for (size_t i = 0; i < hex.length(); i += 2) {
            char high = std::toupper(hex[i]);
            char low = std::toupper(hex[i + 1]);

            if (!std::isxdigit(high) || !std::isxdigit(low)) {
                throw std::invalid_argument("Invalid hex character");
            }

            value_type byte = (hexCharToInt(high) << 4) | hexCharToInt(low);
            result.data_.push_back(byte);
        }

        return result;
    }

  private:
    std::vector<value_type> data_;

    static value_type hexCharToInt(char c) {
        if (c >= '0' && c <= '9')
            return c - '0';
        if (c >= 'A' && c <= 'F')
            return c - 'A' + 10;
        if (c >= 'a' && c <= 'f')
            return c - 'a' + 10;
        throw std::invalid_argument("Invalid hex character");
    }
};

}  // namespace msh::utils

#endif  // MSH_UTILS_BYTE_ARRAY_HPP