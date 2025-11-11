#ifndef RENDERER_H
#define RENDERER_H

#include "Window.h"

#include <SDL3/SDL.h>

class Renderer
{
public:
    bool create(Window& window);
    void clear();

private:
    SDL_Renderer* sdlRenderer_ = nullptr;
};

#endif // RENDERER_H