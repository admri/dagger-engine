#ifndef TEXTURE_H
#define TEXTURE_H

#include "Renderer.h"

#include <SDL3/SDL_render.h>

#include <string>

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

    void render(Renderer &renderer, float x, float y, const SDL_FRect* srcRect = nullptr, const SDL_FRect* dstRect = nullptr) const;
    
    SDL_Texture* getSDLTexture() const { return sdlTexture_.get(); }

private:
    std::unique_ptr<SDL_Texture, SDLTextureDeleter> sdlTexture_;
    float width_ = 0.0f;
    float height_ = 0.0f;
};

#endif // TEXTURE_H
