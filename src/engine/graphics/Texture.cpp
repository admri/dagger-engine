#include <graphics/Texture.h>

#include <graphics/Renderer.h>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>

namespace engine
{

void SDLTextureDeleter::operator()(SDL_Texture *texture) const
{
    if (texture) SDL_DestroyTexture(texture);
}

bool Texture::createFromSurface(const Renderer& renderer, SDL_Surface& surface)
{
    sdlTexture_.reset();

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer.getSDLRenderer(), &surface);
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

bool Texture::loadFromFile(const Renderer& renderer, const std::string& path)
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
    if (!sdlTexture_)
    {
        return;
    }

    sdlTexture_.reset();
}

} // namespace engine
