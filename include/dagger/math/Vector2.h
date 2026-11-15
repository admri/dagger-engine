#ifndef DAGGER_MATH_VECTOR_2_H
#define DAGGER_MATH_VECTOR_2_H

#include <cmath>
#include <concepts>
#include <functional>
#include <limits>

namespace dagger
{
namespace math
{

/// @brief A templated 2D mathematical vector used for positions, directions, and velocities.
template <typename T>
requires std::integral<T> || std::floating_point<T>
struct Vector2
{
    T x{0};
    T y{0};

    Vector2() = default;
    Vector2(T x, T y) : x(x), y(y) {}

    constexpr bool operator==(const Vector2& other) const 
    { 
        if constexpr (std::floating_point<T>)
        {
            return std::abs(x - other.x) <= std::numeric_limits<T>::epsilon() && 
                   std::abs(y - other.y) <= std::numeric_limits<T>::epsilon();
        }
        else
        {
            return x == other.x && y == other.y; 
        }
    }
    constexpr bool operator!=(Vector2 other) const { return !(*this == other); }

    constexpr Vector2& operator+=(Vector2 other) { x += other.x; y += other.y; return *this; }
    constexpr Vector2& operator-=(Vector2 other) { x -= other.x; y -= other.y; return *this; }
    constexpr Vector2& operator*=(T scalar) { x *= scalar; y *= scalar; return *this; }
    constexpr Vector2& operator/=(T scalar) { x /= scalar; y /= scalar; return *this; }

    constexpr Vector2 operator+(Vector2 other) const { auto copy = *this; return copy += other; }
    constexpr Vector2 operator-(Vector2 other) const { auto copy = *this; return copy -= other; }
    constexpr Vector2 operator*(T scalar) const { auto copy = *this; return copy *= scalar; }
    constexpr Vector2 operator/(T scalar) const { auto copy = *this; return copy /= scalar; }
    constexpr Vector2 operator-() const { return {-x, -y}; }

    auto length() const { return std::sqrt(x * x + y * y); }
    constexpr T lengthSquared() const { return x * x + y * y; }
    constexpr T manhattanDistance() const { return std::abs(x) + std::abs(y); }
};

using Vec2f = Vector2<float>;
using Vec2i = Vector2<int>;
using Vec2d = Vector2<double>;

} // namespace math
} // namespace dagger

namespace std
{
    // Hashing method for std::unordered_map
    template <typename T>
    requires std::integral<T> || std::floating_point<T>
    struct hash<dagger::math::Vector2<T>>
    {
        size_t operator()(const dagger::math::Vector2<T>& v) const
        {
            return std::hash<T>()(v.x) ^ (std::hash<T>()(v.y) << 1);
        }
    };
}

#endif // DAGGER_MATH_VECTOR_2_H
