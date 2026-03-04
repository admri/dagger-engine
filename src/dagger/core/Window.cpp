#include <core/Window.h>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>

namespace dagger
{

bool Window::create(const std::string& title, int width, int height, SDL_WindowFlags flags)
{
    SDL_Window* window = SDL_CreateWindow(title.c_str(), width, height, flags);
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
