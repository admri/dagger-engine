#include "Texture.h"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>

bool Texture::loadFromFile(Renderer &renderer, const std::string& path)
{
    sdlTexture_.reset();

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image %s: %s\n", path.c_str(), SDL_GetError());
        return false;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer.getSDLRenderer(), surface);
    SDL_DestroySurface(surface);

    if (!texture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create texture from surface for %s: %s\n", path.c_str(), SDL_GetError());
        return false;
    }

    sdlTexture_.reset(texture);
    SDL_GetTextureSize(sdlTexture_.get(), &width_, &height_);
    
    return true;
}

void Texture::destroy()
{
    if (!sdlTexture_)
    {
        return;
    }

    sdlTexture_.reset();
}

void Texture::render(Renderer &renderer, float x, float y, const SDL_FRect* srcRect, const SDL_FRect* dstRect) const
{
    if (!sdlTexture_ || !renderer.getSDLRenderer())
    {
        return;
    }

    SDL_FRect dst;
    if (dstRect)
    {
        dst = *dstRect;
    }
    else
    {
        dst.x = x;
        dst.y = y;
        dst.w = width_;
        dst.h = height_;
    }

    if (!SDL_RenderTexture(renderer.getSDLRenderer(), sdlTexture_.get(), srcRect, &dst))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to render texture: %s\n", SDL_GetError());
    }
}
