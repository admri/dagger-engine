#include <graphics/MacHacks.h>

#ifdef __APPLE__
#include <graphics/Renderer.h>

#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>

#import <QuartzCore/CAMetalLayer.h>

namespace dagger
{

void ApplyMetalOptimizations(SDL_Renderer* renderer)
{
    if (!renderer) return;

    void* layerPtr = SDL_GetRenderMetalLayer(renderer);

    if (layerPtr)
    {
        SDL_Log("Metal Layer found. Applying macOS anti-lag optimizations...");
        CAMetalLayer* metalLayer = (__bridge CAMetalLayer*)layerPtr;

        metalLayer.maximumDrawableCount = 2; // Forbid vsync triple buffering
        metalLayer.displaySyncEnabled = NO;  // Forbid forced vsync
    }
    else
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Couldn't get Metal Layer pointer.");
    }
}

} // namespace dagger

#endif // __APPLE__
