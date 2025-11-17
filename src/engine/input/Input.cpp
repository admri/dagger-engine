#include <input/Input.h>

#include <SDL3/SDL_keyboard.h>

void Input::update()
{
    keyboardState_ = SDL_GetKeyboardState(nullptr);
}

bool Input::isKeyPressed(SDL_Scancode key) const
{
    if (!keyboardState_)
    {
        return false;
    }

    return keyboardState_[key] != 0;
}