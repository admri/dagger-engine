#ifndef DAGGER_GRAPHICS_RENDERER_H
#define DAGGER_GRAPHICS_RENDERER_H

#include <dagger/graphics/Color.h>
#include <dagger/graphics/Rect.h>

#include <memory>

struct SDL_Renderer;

namespace dagger
{

class Texture;
class Window;

struct SDLRendererDeleter
{
    void operator()(SDL_Renderer* renderer) const;
};

class Renderer
{
    friend class Texture;
    friend class Application;

public:
    bool create(const Window& window);
    void destroy();

    void clear(Color clearColor = Colors::Black);
    void present();

    void drawTexture(const Texture& texture, const Rect& dstRect, const Rect* srcRect = nullptr, bool flipX = false, Color tint = Colors::White);

    void toggleVSync();
    int getWidth() const;
    int getHeight() const;

private:
    SDL_Renderer* get() const { return sdlRenderer_.get(); }

    std::unique_ptr<SDL_Renderer, SDLRendererDeleter> sdlRenderer_;
};

} // namespace dagger

#endif // DAGGER_GRAPHICS_RENDERER_H
