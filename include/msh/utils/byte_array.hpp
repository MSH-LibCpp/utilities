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

    explicit ByteArray(const size_type size, const value_type value = 0) : m_data(size, value) {}

    ByteArray(const value_type* data, const size_type size) : m_data(data, data + size) {}

    ByteArray(const std::initializer_list<value_type> init) : m_data(init) {}

    explicit ByteArray(const std::string& str)
        : m_data(reinterpret_cast<const value_type*>(str.data()),
                 reinterpret_cast<const value_type*>(str.data()) + str.size()) {}

    // Copy operations
    ByteArray(const ByteArray&) = default;
    ByteArray& operator=(const ByteArray&) = default;

    // Move operations
    ByteArray(ByteArray&&) noexcept = default;
    ByteArray& operator=(ByteArray&&) noexcept = default;

    // Element access
    value_type& at(const size_type pos) {
        return m_data.at(pos);
    }
    const value_type& at(const size_type pos) const {
        return m_data.at(pos);
    }

    value_type& operator[](const size_type pos) {
        return m_data[pos];
    }
    const value_type& operator[](const size_type pos) const {
        return m_data[pos];
    }

    value_type& back() {
        if (empty()) {
            throw std::out_of_range("ByteArray is empty");
        }
        return m_data.back();
    }
    const value_type& back() const {
        if (empty()) {
            throw std::out_of_range("ByteArray is empty");
        }
        return m_data.back();
    }

    value_type* data() noexcept {
        return m_data.data();
    }
    const value_type* data() const noexcept {
        return m_data.data();
    }

    // Iterators
    iterator begin() noexcept {
        return m_data.begin();
    }
    const_iterator begin() const noexcept {
        return m_data.begin();
    }
    const_iterator cbegin() const noexcept {
        return m_data.cbegin();
    }

    iterator end() noexcept {
        return m_data.end();
    }
    const_iterator end() const noexcept {
        return m_data.end();
    }
    const_iterator cend() const noexcept {
        return m_data.cend();
    }

    // Capacity
    bool empty() const noexcept {
        return m_data.empty();
    }
    size_type size() const noexcept {
        return m_data.size();
    }
    void reserve(const size_type new_cap) {
        m_data.reserve(new_cap);
    }
    size_type capacity() const noexcept {
        return m_data.capacity();
    }

    // Modifiers
    void clear() noexcept {
        m_data.clear();
    }

    iterator insert(const_iterator pos, const value_type& value) {
        return m_data.insert(pos, value);
    }

    iterator insert(const_iterator pos, value_type&& value) {
        return m_data.insert(pos, std::move(value));
    }

    iterator insert(const_iterator pos, size_type count, const value_type& value) {
        return m_data.insert(pos, count, value);
    }

    template<class InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last) {
        return m_data.insert(pos, first, last);
    }

    iterator insert(const_iterator pos, std::initializer_list<value_type> ilist) {
        return m_data.insert(pos, ilist);
    }

    void append(const ByteArray& other) {
        m_data.insert(m_data.end(), other.begin(), other.end());
    }

    void append(const value_type* data, const size_type size) {
        m_data.insert(m_data.end(), data, data + size);
    }

    void resize(size_type count) {
        m_data.resize(count);
    }
    void resize(size_type count, value_type value) {
        m_data.resize(count, value);
    }

    // Comparison operators
    bool operator==(const ByteArray& other) const {
        return m_data == other.m_data;
    }

    bool operator!=(const ByteArray& other) const {
        return !(*this == other);
    }

    // Utility functions
    std::string toHexString() const {
        static const char hex_chars[] = "0123456789ABCDEF";
        std::string result;
        result.reserve(size() * 2);

        for (value_type byte : m_data) {
            result.push_back(hex_chars[byte >> 4]);
            result.push_back(hex_chars[byte & 0x0F]);
        }

        return result;
    }

    std::string string() const {
        return std::string(reinterpret_cast<const char*>(m_data.data()), m_data.size());
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
            result.m_data.push_back(byte);
        }

        return result;
    }

  private:
    std::vector<value_type> m_data;

    static value_type hexCharToInt(const char c) {
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