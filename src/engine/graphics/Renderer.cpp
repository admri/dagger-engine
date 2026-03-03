#include <graphics/Renderer.h>

#include <core/Window.h>
#include <graphics/Texture.h>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>

namespace engine
{

void SDLRendererDeleter::operator()(SDL_Renderer *renderer) const
{
    if (renderer) SDL_DestroyRenderer(renderer);
}

bool Renderer::create(const Window& window)
{
    SDL_Window* sdlWindow = window.getSDLWindow();
    if (!sdlWindow)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window not created yet.");
        return false;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(sdlWindow, NULL);
    if (!renderer)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create SDL_Renderer: %s", SDL_GetError());
        return false;
    }

    sdlRenderer_.reset(renderer);
    return true;
}

void Renderer::destroy()
{
    if (!sdlRenderer_)
    {
        return;
    }

    sdlRenderer_.reset();
}

void Renderer::clear(Color clearColor)
{
    if (!sdlRenderer_)
    {
        return;
    }

    SDL_SetRenderDrawColor(sdlRenderer_.get(), clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    SDL_RenderClear(sdlRenderer_.get());
}

void Renderer::present()
{
    if (!sdlRenderer_)
    {
        return;
    }

    SDL_RenderPresent(sdlRenderer_.get());
}

void Renderer::drawTexture(const Texture& texture, float x, float y, const SDL_FRect* srcRect)
{
    if (!sdlRenderer_ || !texture.getSDLTexture())
    {
        return;
    }

    SDL_FRect dstRect = {
        x, 
        y, 
        (srcRect ? srcRect->w : texture.getWidth()),
        (srcRect ? srcRect->h : texture.getHeight())
    };

    if (!SDL_RenderTexture(sdlRenderer_.get(), texture.getSDLTexture(), srcRect, &dstRect))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to render texture: %s", SDL_GetError());
    }
}

void Renderer::toggleVSync()
{
    if (!sdlRenderer_)
    {
        return;
    }

    int vsync = 0;
    SDL_GetRenderVSync(sdlRenderer_.get(), &vsync);
    
    SDL_SetRenderVSync(sdlRenderer_.get(), vsync ? SDL_RENDERER_VSYNC_DISABLED : 1);
}

} // namespace engine
