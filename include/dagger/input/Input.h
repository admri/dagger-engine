#ifndef DAGGER_INPUT_INPUT_H
#define DAGGER_INPUT_INPUT_H

#include <array>

namespace dagger
{

enum class Key;
constexpr int MAX_KEYS = 512;

class Input
{
public:
    void init();
    void update();
    
    bool isKeyPressed(Key key) const;
    bool isKeyDown(Key key) const;

    bool isLeftMouseDown() const;
    void getMousePosition(int* x, int* y) const;

private:
    const bool* keyboardState_ = nullptr;
    std::array<bool, MAX_KEYS> previousKeyboardState_ = {};
};

} // namespace dagger

#endif // DAGGER_INPUT_INPUT_H