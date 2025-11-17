#ifndef TEXTURE_H
#define TEXTURE_H

#include <graphics/Renderer.h>

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

#include <memory>
#include <string>

class Renderer;

struct SDLTextureDeleter
{
    void operator()(SDL_Texture* texture) const
    {
        if (texture) SDL_DestroyTexture(texture);
    }
};

class Texture
{
public:
    bool loadFromFile(Renderer& renderer, const std::string& path);
    void destroy();

    void render(Renderer& renderer, float x, float y, const SDL_FRect* srcRect = nullptr, const SDL_FRect* dstRect = nullptr) const;
    
    SDL_Texture* getSDLTexture() const { return sdlTexture_.get(); }
    float getWidth() const { return width_; }
    float getHeight() const { return height_; }

private:
    std::unique_ptr<SDL_Texture, SDLTextureDeleter> sdlTexture_;
    float width_ = 0.0f;
    float height_ = 0.0f;
};

#endif // TEXTURE_H
