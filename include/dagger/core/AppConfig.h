#ifndef DAGGER_CORE_APP_CONFIG_H
#define DAGGER_CORE_APP_CONFIG_H

#include <dagger/utils/EnumBitmask.h>

#include <cstdint>
#include <string>

namespace dagger
{

enum class WindowFlags : uint32_t
{
    None             = 0,
    Fullscreen       = 1 << 0,
    Resizable        = 1 << 1,
    Borderless       = 1 << 2,
    Hidden           = 1 << 3,
    HighPixelDensity = 1 << 4
};

template <> constexpr bool enable_bitmask_operators<WindowFlags> = true;

struct AppConfig
{
    std::string title = "Dagger Application";
    int width = 800;
    int height = 600;
    WindowFlags flags = WindowFlags::Resizable;
    bool showFPS = true;
    bool vsync = true;
    double tps = 60;
};

} // namespace dagger

#endif // DAGGER_CORE_APP_CONFIG_H
