#ifndef DAGGER_INPUT_INPUT_H
#define DAGGER_INPUT_INPUT_H

#include <SDL3/SDL_scancode.h>

#include <array>

namespace dagger
{

enum class Key;

class Input
{
public:
    void init();
    void update();
    bool isKeyPressed(Key key) const;
    bool isKeyDown(Key key) const;

private:
    const bool* keyboardState_ = nullptr;
    std::array<bool, SDL_SCANCODE_COUNT> previousKeyboardState_ = {};
};

} // namespace dagger

#endif // DAGGER_INPUT_INPUT_H