#ifndef RENDERER_H
#define RENDERER_H

#include "Color.h"
#include "Texture.h"
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

class Renderer
{
public:
    bool create(const Window& window);
    void destroy();

    void clear(const Color& clearColor = Colors::Black);
    void present();
    void renderTexture(const Texture& texture, float x, float y, const SDL_FRect* srcRect = nullptr, const SDL_FRect* dstRect = nullptr);

    void toggleVSync();

    inline SDL_Renderer* getSDLRenderer() const { return sdlRenderer_.get(); }

private:
    std::unique_ptr<SDL_Renderer, SDLRendererDeleter> sdlRenderer_;
};

#endif // RENDERER_H