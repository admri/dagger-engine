#ifndef DAGGER_INPUT_KEY_CODES_H
#define DAGGER_INPUT_KEY_CODES_H

namespace dagger
{

/// @brief Standardized engine key codes abstracting the underlying hardware API.
enum class Key
{
    // Letters
    A, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

    // Numbers
    Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,

    // Controls
    Escape, Space, Enter, Tab, Backspace,

    // Arrows
    Right, Left, Down, Up,

    // Modifiers
    LShift, RShift, LCtrl, RCtrl, LAlt, RAlt,

    // Function keys
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12
};

} // namespace dagger

#endif // DAGGER_INPUT_KEY_CODES_H
