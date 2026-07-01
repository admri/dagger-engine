#ifndef DAGGER_UTILS_STRING_HASH_H
#define DAGGER_UTILS_STRING_HASH_H

#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>

namespace dagger
{

using HashValue = uint32_t;

struct StringHash;

constexpr auto hashString(const char* str, std::size_t length) -> HashValue;
StringHash hashString(const std::string& str);

/// @brief A strongly-typed wrapper around a 32-bit FNV-1a hash.
/// Prevents accidental mixing of hashes with standard integers.
struct StringHash
{
    using ValueType = HashValue;
    ValueType value;
    static constexpr ValueType INVALID_VALUE = 0;

    constexpr StringHash() : value(INVALID_VALUE) {}
    explicit constexpr StringHash(ValueType v) : value(v) {}

    template <std::size_t N>
    consteval StringHash(const char (&str)[N]) : value(hashString(str, N - 1)) {}

    StringHash(const std::string& str) : value(hashString(str.c_str(), str.length())) {}

    constexpr bool operator==(const StringHash& other) const { return value == other.value; }
    constexpr bool operator!=(const StringHash& other) const { return value != other.value; }
    constexpr bool isValid() const { return value != INVALID_VALUE; }
};

namespace detail
{
    constexpr StringHash::ValueType FNV_OFFSET_BASIS = 2166136261u;
    constexpr StringHash::ValueType FNV_PRIME = 16777619u;
}

/// @brief Core FNV-1a hash algorithm for fast string hashing. Can be evaluated at compile-time.
constexpr auto hashString(const char* str, std::size_t length) -> StringHash::ValueType
{
    StringHash::ValueType hash = detail::FNV_OFFSET_BASIS;
    for (std::size_t i = 0; i < length; ++i)
    {
        hash ^= static_cast<StringHash::ValueType>(str[i]);
        hash *= detail::FNV_PRIME;
    }
    return hash;
}

/// @brief hashString overload for hashing std::string objects at runtime.
inline StringHash hashString(const std::string& str)
{
    return StringHash(hashString(str.c_str(), str.length()));
}

inline namespace literals
{
    /// @brief Compile-time literal for hashing strings (e.g., "my_sound"_hash).
    consteval StringHash operator"" _hash(const char* str, std::size_t length)
    {
        return StringHash{hashString(str, length)};
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
