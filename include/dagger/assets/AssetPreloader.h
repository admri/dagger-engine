#ifndef DAGGER_ASSETS_ASSET_PRELOADER_H
#define DAGGER_ASSETS_ASSET_PRELOADER_H

#include <nlohmann/json_fwd.hpp>

#include <string>

namespace dagger
{

class AssetManager;

class AssetPreloader
{
public:
    static void loadFromManifest(const std::string& manifestPath, AssetManager& assets);
};

}

#endif // DAGGER_ASSETS_ASSET_PRELOADER_H
