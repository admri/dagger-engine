#ifndef RENDERER_H
#define RENDERER_H

#include <assets/Font.h>
#include <core/Window.h>
#include <graphics/Color.h>
#include <graphics/Texture.h>

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>

#include <memory>

class Texture;

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

    void drawTexture(const Texture& texture, const SDL_FRect* srcRect, const SDL_FRect* dstRect);
    void drawText(const Font& font, const std::string& text, float x, float y, const Color& color);

    void toggleVSync();

    inline SDL_Renderer* getSDLRenderer() const { return sdlRenderer_.get(); }

private:
    std::unique_ptr<SDL_Renderer, SDLRendererDeleter> sdlRenderer_;
};

#endif // RENDERER_H