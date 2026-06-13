#ifndef DAGGER_ASSETS_ASSET_PRELOADER_H
#define DAGGER_ASSETS_ASSET_PRELOADER_H

#include <dagger/assets/AssetManager.h>
#include <dagger/assets/Font.h>
#include <dagger/graphics/Texture.h>

#include <nlohmann/json.hpp>

#include <fstream>
#include <stdexcept>
#include <string>

namespace dagger
{

class AssetPreloader
{
public:
    static void loadFromManifest(const std::string& manifestPath, AssetManager& assets)
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
    }
};

}

#endif // DAGGER_ASSETS_ASSET_PRELOADER_H
