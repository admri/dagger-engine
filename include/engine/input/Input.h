#ifndef INPUT_H
#define INPUT_H

#include <SDL3/SDL_scancode.h>

#include <array>

namespace engine
{

class Input
{
public:
    void update();
    bool isKeyPressed(SDL_Scancode key) const;
    bool isKeyDown(SDL_Scancode key) const;

private:
    const bool* keyboardState_ = nullptr;
    std::array<bool, SDL_SCANCODE_COUNT> previousKeyboardState_ = {};
};

} // namespace engine

#endif // INPUT_H