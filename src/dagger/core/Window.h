#ifndef DAGGER_CORE_WINDOW_H
#define DAGGER_CORE_WINDOW_H

#include <core/AppConfig.h>

#include <memory>
#include <string>

struct SDL_Window;

namespace dagger
{

struct SDLWindowDeleter
{
    void operator()(SDL_Window* window) const;
};

class Window
{
public:
    bool create(const std::string& title, int width, int height, WindowFlags flags);
    void destroy();

    void setTitle(const std::string& title);
    
    inline SDL_Window* getSDLWindow() const { return sdlWindow_.get(); };

private:
    std::unique_ptr<SDL_Window, SDLWindowDeleter> sdlWindow_;
};

} // namespace dagger

#endif // DAGGER_CORE_WINDOW_H
