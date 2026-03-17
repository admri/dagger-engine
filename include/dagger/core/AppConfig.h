#ifndef DAGGER_CORE_APP_CONFIG_H
#define DAGGER_CORE_APP_CONFIG_H

#include <cstdint>
#include <string>

namespace dagger
{

enum class WindowFlags : uint64_t
{
    None             = 0,
    Fullscreen       = 1 << 0,
    Resizable        = 1 << 1,
    Borderless       = 1 << 2,
    Hidden           = 1 << 3,
    HighPixelDensity = 1 << 4
};

inline WindowFlags operator|(WindowFlags a, WindowFlags b) 
{
    return static_cast<WindowFlags>(static_cast<uint64_t>(a) | static_cast<uint64_t>(b));
}

inline bool hasFlag(WindowFlags mask, WindowFlags flag)
{
    return static_cast<uint64_t>(mask) & static_cast<uint64_t>(flag);
}

struct AppConfig
{
    std::string title = "Engine Game";
    int width = 800;
    int height = 600;
    WindowFlags flags = WindowFlags::Resizable;
    bool showFPS = true;
    bool vsync = true;
};

} // namespace dagger

#endif // DAGGER_CORE_APP_CONFIG_H
