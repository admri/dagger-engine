#include <core/Window.h>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_video.h>

namespace dagger
{

namespace
{

bool has(WindowFlags mask, WindowFlags flag)
{
    return (static_cast<uint64_t>(mask) & static_cast<uint64_t>(flag)) != 0;
}

Uint64 toSDL(WindowFlags flags)
{
    Uint64 sdlFlags = 0;

    if (has(flags, WindowFlags::Fullscreen))       sdlFlags |= SDL_WINDOW_FULLSCREEN;
    if (has(flags, WindowFlags::Resizable))        sdlFlags |= SDL_WINDOW_RESIZABLE;
    if (has(flags, WindowFlags::Borderless))       sdlFlags |= SDL_WINDOW_BORDERLESS;
    if (has(flags, WindowFlags::Hidden))           sdlFlags |= SDL_WINDOW_HIDDEN;
    if (has(flags, WindowFlags::HighPixelDensity)) sdlFlags |= SDL_WINDOW_HIGH_PIXEL_DENSITY;
    
    return sdlFlags;
}

} // namespace

void SDLWindowDeleter::operator()(SDL_Window* window) const
{
    if (window) SDL_DestroyWindow(window);
}

bool Window::create(const std::string& title, int width, int height, WindowFlags flags)
{
    SDL_Window* window = SDL_CreateWindow(title.c_str(), width, height, toSDL(flags));
    if (!window)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create SDL_Window: %s", SDL_GetError());
        return false;
    }

    sdlWindow_.reset(window);
    return true;
}

void Window::destroy()
{
    if (!sdlWindow_)
    {
        return;
    }

    sdlWindow_.reset();
}

void Window::setTitle(const std::string &title)
{
    if (!sdlWindow_)
    {
        return;
    }

    SDL_SetWindowTitle(sdlWindow_.get(), title.c_str());
}

} // namespace dagger
