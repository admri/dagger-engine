#ifndef DAGGER_GRAPHICS_COLOR_H
#define DAGGER_GRAPHICS_COLOR_H

#include <dagger/core/Serialization.h>

#include <cstdint>

namespace dagger
{

/// @brief A standard 32-bit RGBA color structure (0-255 per channel).
struct Color
{
    uint8_t r = 255;
    uint8_t g = 255;
    uint8_t b = 255;
    uint8_t a = 255;

    Color() = default;

    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
        : r(r), g(g), b(b), a(a)
    {
    }

    bool operator==(Color other) const
    {
        return this->r == other.r
            && this->g == other.g
            && this->b == other.b
            && this->a == other.a;
    }

    bool operator!=(Color other) const
    {
        return !(*this == other);
    }
};

namespace Colors
{
    inline Color Black{  0,   0,   0};
    inline Color White{255, 255, 255};
    inline Color Red  {255,   0,   0};
    inline Color Green{  0, 255,   0};
    inline Color Blue {  0,   0, 255};
}

DAGGER_SERIALIZE_COMPONENT(Color, r, g, b, a)

} // namespace dagger

#endif // DAGGER_GRAPHICS_COLOR_H