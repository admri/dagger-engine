#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL_video.h>

#include <memory>
#include <string>

struct SDLWindowDeleter
{
    void operator()(SDL_Window* window) const
    {
        if (window) SDL_DestroyWindow(window);
    }
};

class Window
{
public:
    bool create(const std::string& title, int width, int height, SDL_WindowFlags flags);
    void destroy();

    void setTitle(const std::string& title);
    
    inline SDL_Window* getSDLWindow() const { return sdlWindow_.get(); };

private:
    std::unique_ptr<SDL_Window, SDLWindowDeleter> sdlWindow_;
};

#endif // WINDOW_H
