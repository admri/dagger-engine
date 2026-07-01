#ifndef DAGGER_INPUT_INPUT_H
#define DAGGER_INPUT_INPUT_H

#include <array>

namespace dagger
{

enum class Key;
constexpr int MAX_KEYS = 512;

/// @brief Core hardware service for polling keyboard and mouse states.
class Input
{
public:
    void init();

    /// @brief Captures the current hardware input state. Called automatically by Application.
    void update();
    
    /// @brief Checks if a key was pressed exactly this frame (triggers once per press).
    bool isKeyPressed(Key key) const;
    
    /// @brief Checks if a key is currently being held down (triggers continuously).
    bool isKeyDown(Key key) const;

    bool isLeftMouseDown() const;
    
    /// @brief Retrieves the current mouse coordinates relative to the window.
    void getMousePosition(int* x, int* y) const;

private:
    const bool* keyboardState_ = nullptr;
    std::array<bool, MAX_KEYS> previousKeyboardState_ = {};
};

} // namespace dagger

#endif // DAGGER_INPUT_INPUT_H