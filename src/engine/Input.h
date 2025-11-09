#pragma once

#include <SDL3/SDL.h>

class Input
{
public:
    void update();
    bool isKeyPressed(SDL_Scancode key) const;

private:
    const Uint8* keyboardState_ = nullptr;
};