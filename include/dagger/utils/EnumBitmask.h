#ifndef DAGGER_UTILS_ENUM_BITMASK_H
#define DAGGER_UTILS_ENUM_BITMASK_H

#include <type_traits>

namespace dagger
{

template <typename T>
constexpr bool enable_bitmask_operators = false;

template <typename T>
concept BitmaskEnum = std::is_enum_v<T> && enable_bitmask_operators<T>;

template <BitmaskEnum T>
constexpr T operator|(T a, T b)
{
    using U = std::underlying_type_t<T>;
    return static_cast<T>(static_cast<U>(a) | static_cast<U>(b));
}

template <BitmaskEnum T>
constexpr T operator&(T a, T b)
{
    using U = std::underlying_type_t<T>;
    return static_cast<T>(static_cast<U>(a) & static_cast<U>(b));
}

template <BitmaskEnum T>
constexpr T operator^(T a, T b)
{
    using U = std::underlying_type_t<T>;
    return static_cast<T>(static_cast<U>(a) ^ static_cast<U>(b));
}

template <BitmaskEnum T>
constexpr T operator~(T a)
{
    using U = std::underlying_type_t<T>;
    return static_cast<T>(~static_cast<U>(a));
}

template <BitmaskEnum T>
constexpr T& operator|=(T& a, T b) { return a = a | b; }

template <BitmaskEnum T>
constexpr T& operator&=(T& a, T b) { return a = a & b; }

template <BitmaskEnum T>
constexpr T& operator^=(T& a, T b) { return a = a ^ b; }

template <BitmaskEnum T>
constexpr bool hasFlag(T mask, T flag)
{
    using U = std::underlying_type_t<T>;
    return (static_cast<U>(mask) & static_cast<U>(flag)) != 0;
}

} // namespace dagger

#endif // DAGGER_UTILS_ENUM_BITMASK_H
