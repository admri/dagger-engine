#ifndef DAGGER_MATH_VECTOR_2_H
#define DAGGER_MATH_VECTOR_2_H

#include <cmath>
#include <concepts>
#include <functional>

namespace dagger
{
namespace math
{

/// @brief A templated 2D mathematical vector used for positions, directions, and velocities.
template <typename T>
requires std::integral<T> || std::floating_point<T>
struct Vector2
{
    T x, y;

    Vector2() : x(0), y(0) {}
    Vector2(T x, T y) : x(x), y(y) {}

    bool operator==(Vector2 other) const { return x == other.x && y == other.y; }
    bool operator!=(Vector2 other) const { return !(*this == other); }

    Vector2 operator+(Vector2 other) const { return {x + other.x, y + other.y}; }
    Vector2 operator-(Vector2 other) const { return {x - other.x, y - other.y}; }
    Vector2 operator*(T scalar) const { return {x * scalar, y * scalar}; }
    Vector2 operator/(T scalar) const { return {x / scalar, y / scalar}; }

    float length() const { return std::sqrt(static_cast<T>(x * x + y * y)); }
};

using Vec2f = Vector2<float>;
using Vec2i = Vector2<int>;

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
