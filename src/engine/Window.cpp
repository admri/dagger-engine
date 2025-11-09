#include "Window.h"

bool Window::create(const std::string& title, int width, int height, SDL_WindowFlags flags)
{
    sdlWindow_ = SDL_CreateWindow(title.c_str(), width, height, flags);
    return sdlWindow_ != nullptr;
}

void Window::destroy()
{
    if (sdlWindow_)
    {
        SDL_DestroyWindow(sdlWindow_);
        sdlWindow_ = nullptr;
    }
}