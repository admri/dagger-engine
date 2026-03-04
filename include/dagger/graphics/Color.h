#ifndef DAGGER_GRAPHICS_COLOR_H
#define DAGGER_GRAPHICS_COLOR_H

#include <SDL3/SDL_pixels.h>

namespace dagger
{

struct Color : SDL_Color
{
    Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255)
        : SDL_Color{ r, g, b, a }
    {
    }

    bool operator==(Color other) const
    {
        return this->r == other.r
            && this->g == other.g
            && this->b == other.b
            && this->a == other.a;
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