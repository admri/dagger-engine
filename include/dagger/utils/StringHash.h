#ifndef DAGGER_UTILS_STRING_HASH_H
#define DAGGER_UTILS_STRING_HASH_H

#include <cstddef>
#include <cstdint>
#include <functional>
#include <string_view>

namespace dagger
{

using HashValue = uint32_t;

namespace detail
{
    constexpr HashValue FNV_OFFSET_BASIS = 2166136261u;
    constexpr HashValue FNV_PRIME = 16777619u;

    /// @brief Core FNV-1a hash algorithm for fast string hashing. Can be evaluated at compile-time.
    constexpr auto hashString(const char* str, std::size_t length) -> HashValue
    {
        HashValue hash = FNV_OFFSET_BASIS;
        for (std::size_t i = 0; i < length; ++i)
        {
            hash ^= static_cast<HashValue>(str[i]);
            hash *= FNV_PRIME;
        }
        return hash;
    }
}

/// @brief A strongly-typed wrapper around a 32-bit FNV-1a hash.
/// Prevents accidental mixing of hashes with standard integers.
struct StringHash
{
    using ValueType = HashValue;
    ValueType value;
    static constexpr ValueType INVALID_VALUE = 0;

    constexpr StringHash() : value(INVALID_VALUE) {}
    explicit constexpr StringHash(ValueType v) : value(v) {}

    // Implicit compile-time hashing
    template <std::size_t N>
    consteval StringHash(const char (&str)[N]) : value(detail::hashString(str, N - 1)) {}

    // Explicit runtime hashing
    explicit constexpr StringHash(std::string_view str) : value(detail::hashString(str.data(), str.length())) {}

    constexpr bool operator==(const StringHash& other) const { return value == other.value; }
    constexpr bool operator!=(const StringHash& other) const { return value != other.value; }
    constexpr bool isValid() const { return value != INVALID_VALUE; }
};

inline namespace literals
{
    /// @brief Compile-time literal for hashing strings (e.g., "my_sound"_hash).
    consteval StringHash operator""_hash(const char* str, std::size_t length)
    {
        return StringHash{detail::hashString(str, length)};
    }
}

} // namespace dagger

namespace std
{
    template <>
    struct hash<dagger::StringHash>
    {
        size_t operator()(const dagger::StringHash& h) const
        {
            return std::hash<dagger::StringHash::ValueType>()(h.value);
        }
    };
}

#endif // DAGGER_UTILS_STRING_HASH_H
