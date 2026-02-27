#ifndef RENDERER_H
#define RENDERER_H

#include <graphics/Color.h>

#include <memory>

struct SDL_Renderer;
struct SDL_FRect;

class Texture;
class Window;

struct SDLRendererDeleter
{
    void operator()(SDL_Renderer* renderer) const;
};

class Renderer
{
public:
    bool create(const Window& window);
    void destroy();

    void clear(Color clearColor = Colors::Black);
    void present();

    void drawTexture(const Texture& texture, float x, float y, const SDL_FRect* srcRect = nullptr);

    void toggleVSync();

    SDL_Renderer* getSDLRenderer() const { return sdlRenderer_.get(); }

private:
    std::unique_ptr<SDL_Renderer, SDLRendererDeleter> sdlRenderer_;
};

#endif // RENDERER_H
