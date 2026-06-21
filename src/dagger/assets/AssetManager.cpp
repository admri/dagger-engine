#include <dagger/assets/AssetManager.h>

#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_log.h>

namespace dagger
{

AssetManager::AssetManager(Renderer& renderer, AudioManager& audio)
    : renderer_(renderer)
    , audio_(audio)
{
    const char* basePath = SDL_GetBasePath();
    if (basePath)
    {
        basePath_ = std::string(basePath) + "assets/";
    }
    else 
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Failed to get base path: %s",
                     SDL_GetError());
        basePath_ = "assets/";
    }
}

} // namespace dagger
