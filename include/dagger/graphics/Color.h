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

    constexpr Color() = default;

    constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
        : r(r), g(g), b(b), a(a)
    {
    }

    constexpr explicit Color(uint32_t hex)
        : r((hex >> 24) & 0xFF),
          g((hex >> 16) & 0xFF),
          b((hex >> 8)  & 0xFF),
          a( hex        & 0xFF)
    {
    }

    constexpr bool operator==(const Color&) const = default;
};

static_assert(sizeof(Color) == 4, "dagger::Color must be exactly 4 bytes to map to SDL_Color");

namespace Colors
{
    inline constexpr Color Black{  0,   0,   0};
    inline constexpr Color White{255, 255, 255};
    inline constexpr Color Red  {255,   0,   0};
    inline constexpr Color Green{  0, 255,   0};
    inline constexpr Color Blue {  0,   0, 255};
    inline constexpr Color Transparent{0, 0, 0, 0};
}

DAGGER_SERIALIZE_COMPONENT(Color, r, g, b, a)

} // namespace dagger

#endif // DAGGER_GRAPHICS_COLOR_H