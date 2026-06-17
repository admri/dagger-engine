#ifndef DAGGER_UTILS_STRING_HASH_H
#define DAGGER_UTILS_STRING_HASH_H

#include <cstddef>
#include <cstdint>

namespace dagger
{

namespace detail
{
    constexpr uint32_t FNV_OFFSET_BASIS = 2166136261u;
    constexpr uint32_t FNV_PRIME = 16777619u;
}

// FNV-1a hash algorithm for fast string hashing (32-bits)
constexpr uint32_t hashString(const char* str, std::size_t length)
{
    uint32_t hash = detail::FNV_OFFSET_BASIS;
    for (std::size_t i = 0; i < length; ++i)
    {
        hash ^= static_cast<uint32_t>(str[i]);
        hash *= detail::FNV_PRIME;
    }
    return hash;
}

inline namespace literals
{
    constexpr uint32_t operator"" _hash(const char* str, std::size_t length)
    {
        return hashString(str, length);
    }
}

} // namespace dagger

#endif // DAGGER_UTILS_STRING_HASH_H
