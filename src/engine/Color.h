#ifndef COLOR_H
#define COLOR_H

#include <SDL3/SDL_pixels.h>

struct Color : SDL_Color
{
    Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
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

#endif // COLOR_H