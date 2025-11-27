#include <input/Input.h>

#include <SDL3/SDL_keyboard.h>

#include <algorithm>

void Input::update()
{
    if (keyboardState_)
    {
        std::copy(keyboardState_, keyboardState_ + SDL_SCANCODE_COUNT, previousKeyboardState_.begin());
    }

    keyboardState_ = SDL_GetKeyboardState(nullptr);
}

bool Input::isKeyPressed(SDL_Scancode key) const
{
    if (!keyboardState_)
    {
        return false;
    }

    return keyboardState_[key] && !previousKeyboardState_[key];
}

bool Input::isKeyDown(SDL_Scancode key) const
{
    if (!keyboardState_)
    {
        return false;
    }

    return keyboardState_[key];
}