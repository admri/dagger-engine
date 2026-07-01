#ifndef DAGGER_ASSETS_ASSET_PRELOADER_H
#define DAGGER_ASSETS_ASSET_PRELOADER_H

#include <nlohmann/json_fwd.hpp>

#include <string>

namespace dagger
{

class AssetManager;

/// @brief Utility class for bulk-loading assets defined in a JSON manifest file.
class AssetPreloader
{
public:
    /// @brief Parses a manifest file and loads all defined textures, sounds, and prefabs into the AssetManager.
    /// @param manifestPath Relative path to the JSON manifest.
    /// @param assets The AssetManager instance to load the resources into.
    static void loadFromManifest(const std::string& manifestPath, AssetManager& assets);
};

}

#endif // DAGGER_ASSETS_ASSET_PRELOADER_H
