#include <graphics/Renderer.h>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>

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

void Renderer::clear(const Color& clearColor)
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

void Renderer::drawTexture(const Texture& texture, const SDL_FRect* srcRect, const SDL_FRect* dstRect)
{
    if (!sdlRenderer_ || !texture.getSDLTexture())
    {
        return;
    }

    if (!SDL_RenderTexture(sdlRenderer_.get(), texture.getSDLTexture(), srcRect, dstRect))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to render texture: %s", SDL_GetError());
    }
}

void Renderer::drawText(const Font& font, const std::string& text, float x, float y, const Color& color)
{
    SDL_Surface* textSurface = TTF_RenderText_Blended(font.get(), text.c_str(), text.length(), color);

    if (!textSurface)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create text surface: %s", SDL_GetError());
        return;
    }

    float textWidth = static_cast<float>(textSurface->w);
    float textHeight = static_cast<float>(textSurface->h);

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(sdlRenderer_.get(), textSurface);
    SDL_DestroySurface(textSurface);

    if (!textTexture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create text texture: %s", SDL_GetError());
        return;
    }

    SDL_FRect dstRect = { x, y, textWidth, textHeight };
    if (!SDL_RenderTexture(sdlRenderer_.get(), textTexture, nullptr, &dstRect))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to render text texture: %s", SDL_GetError());
    }

    SDL_DestroyTexture(textTexture);
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