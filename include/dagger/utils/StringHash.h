#ifndef DAGGER_UTILS_STRING_HASH_H
#define DAGGER_UTILS_STRING_HASH_H

#include <cstddef>
#include <cstdint>
#include <functional>

namespace dagger
{

struct StringHash
{
    using ValueType = uint32_t;

    ValueType value;

    static constexpr ValueType INVALID_VALUE = 0;

    constexpr StringHash() : value(INVALID_VALUE) {}

    explicit constexpr StringHash(ValueType v) : value(v) {}

    constexpr bool operator==(const StringHash& other) const { return value == other.value; }
    constexpr bool operator!=(const StringHash& other) const { return value != other.value; }
    
    constexpr bool isValid() const { return value != INVALID_VALUE; }
};

namespace detail
{
    constexpr StringHash::ValueType FNV_OFFSET_BASIS = 2166136261u;
    constexpr StringHash::ValueType FNV_PRIME = 16777619u;
}

// FNV-1a hash algorithm for fast string hashing (32-bits)
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

inline StringHash hashString(const std::string& str)
{
    return StringHash(hashString(str.c_str(), str.length()));
}

inline namespace literals
{
    constexpr StringHash operator"" _hash(const char* str, std::size_t length)
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
