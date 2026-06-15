#ifndef DAGGER_UTILS_STRING_HASH_H
#define DAGGER_UTILS_STRING_HASH_H

#include <cstddef>
#include <cstdint>

namespace dagger
{

constexpr uint32_t hashString(const char* str, std::size_t length)
{
    uint32_t hash = 2166136261u;
    for (std::size_t i = 0; i < length; ++i)
    {
        hash ^= static_cast<uint32_t>(str[i]);
        hash *= 16777619u;
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
