#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>

#include <string>

class Window
{
public:
    bool create(const std::string& title, int width, int height, SDL_WindowFlags flags);
    void destroy();
    SDL_Window* getSDLWindow() const;

private:
    SDL_Window* sdlWindow_ = nullptr;
};

#endif // WINDOW_H