#include <dagger/assets/AssetPreloader.h>

#include <dagger/assets/AssetManager.h>
#include <dagger/assets/Font.h>
#include <dagger/assets/Prefab.h>
#include <dagger/graphics/Texture.h>

#include <nlohmann/json.hpp>

#include <fstream>
#include <stdexcept>

namespace dagger
{
    
void AssetPreloader::loadFromManifest(const std::string& manifestPath, AssetManager& assets)
{
    std::string path = assets.getAbsolutePath(manifestPath);
    std::fstream file(path, std::ios::in);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open asset manifest: " + path);
    }

    nlohmann::json manifest;
    file >> manifest;

    if (manifest.contains("textures"))
    {
        for (const auto& texture : manifest["textures"])
        {
            assets.load<Texture>(
                texture["id"].get<std::string>(),
                texture["path"].get<std::string>()
            );
        }
    }
    
    if (manifest.contains("fonts"))
    {
        for (const auto& font : manifest["fonts"])
        {
            assets.load<Font>(
                font["id"].get<std::string>(),
                font["path"].get<std::string>(),
                font["size"].get<int>()
            );
        }
    }

    if (manifest.contains("prefabs"))
    {
        for (const auto& prefab : manifest["prefabs"])
        {
            assets.load<Prefab>(
                prefab["id"].get<std::string>(),
                prefab["path"].get<std::string>()
            );
        }
    }
}

} // namespace dagger
