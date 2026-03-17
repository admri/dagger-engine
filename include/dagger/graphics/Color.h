#ifndef DAGGER_GRAPHICS_COLOR_H
#define DAGGER_GRAPHICS_COLOR_H

#include <cstdint>

namespace dagger
{

struct Color
{
    uint8_t r, g, b, a;

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

} // namespace dagger

#endif // DAGGER_GRAPHICS_COLOR_H