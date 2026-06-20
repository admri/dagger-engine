#include <graphics/Texture.h>

#include <graphics/Renderer.h>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>

namespace dagger
{

void SDLTextureDeleter::operator()(SDL_Texture *texture) const
{
    if (texture) SDL_DestroyTexture(texture);
}

bool Texture::createFromSurface(const Renderer& renderer, SDL_Surface& surface)
{
    destroy();

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer.get(), &surface);
    if (!texture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Failed to create texture from surface: %s\n",
                     SDL_GetError());
        return false;
    }

    sdlTexture_.reset(texture);
    SDL_GetTextureSize(sdlTexture_.get(), &width_, &height_);

    return true;
}

bool Texture::createRenderTarget(Renderer& renderer, int width, int height)
{
    destroy(); 

    SDL_Texture* texture = SDL_CreateTexture(
        renderer.get(),
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        width, 
        height
    );

    if (!texture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Failed to create render target: %s", SDL_GetError());
        return false;
    }

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    sdlTexture_.reset(texture); 
    width_ = width;
    height_ = height;

    return true;
}

bool Texture::load(const Renderer& renderer, const std::string& path)
{
    sdlTexture_.reset();

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Failed to load image %s: %s\n",
                     path.c_str(),
                     SDL_GetError());
        return false;
    }
    
    bool result = createFromSurface(renderer, *surface);
    SDL_DestroySurface(surface);

    return result;
}

void Texture::destroy()
{
    if (!sdlTexture_) return;
    sdlTexture_.reset();
}

} // namespace dagger
