#ifndef RENDERER_H
#define RENDERER_H

#include "Window.h"

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>

#include <memory>

struct SDLRendererDeleter
{
    void operator()(SDL_Renderer* renderer) const
    {
        if (renderer) SDL_DestroyRenderer(renderer);
    }
};

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

class Renderer
{
public:
    bool create(const Window& window);
    void destroy();

    void clear(const Color& clearColor = {0, 0, 0, 255});
    void present();

    inline SDL_Renderer* getSDLRenderer() const { return sdlRenderer_.get(); }

private:
    std::unique_ptr<SDL_Renderer, SDLRendererDeleter> sdlRenderer_;
};

#endif // RENDERER_H