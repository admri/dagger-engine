#ifndef INPUT_H
#define INPUT_H

#include <SDL3/SDL_scancode.h>

class Input
{
public:
    void update();
    bool isKeyPressed(SDL_Scancode key) const;

private:
    const bool* keyboardState_ = nullptr;
};

#endif // INPUT_H