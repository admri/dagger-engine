#ifndef RENDERER_H
#define RENDERER_H

#include "Window.h"

#include "Color.h"

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

class Renderer
{
public:
    bool create(const Window& window);
    void destroy();

    void clear(const Color& clearColor = Colors::Black);
    void present();

    void toggleVSync();

    inline SDL_Renderer* getSDLRenderer() const { return sdlRenderer_.get(); }

private:
    std::unique_ptr<SDL_Renderer, SDLRendererDeleter> sdlRenderer_;
};

#endif // RENDERER_H