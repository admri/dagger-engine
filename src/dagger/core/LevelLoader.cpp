#include <dagger/core/LevelLoader.h>

#include <dagger/assets/AssetManager.h>
#include <dagger/assets/AssetPreloader.h>
#include <dagger/assets/PrefabFactory.h>
#include <dagger/core/Application.h>
#include <dagger/core/Scene.h>
#include <dagger/core/components/TransformComponent.h>

#include <nlohmann/json.hpp>

#include <fstream>

namespace dagger
{

bool LevelLoader::load(const std::string& filepath, Scene& scene, Application& app)
{
    auto& assets = app.getAssets();
    std::string absolute_path = assets.getAbsolutePath(filepath);

    std::ifstream file(absolute_path);
    if (!file.is_open()) return false;

    nlohmann::json data = nlohmann::json::parse(file, nullptr, false);
    if (data.is_discarded()) return false;

    // Load manifest
    if (data.contains("required_manifest"))
    {
        std::string manifestPath = data["required_manifest"].get<std::string>();
        AssetPreloader::loadFromManifest(manifestPath, assets);
    }

    // Spawn entities from prefabs
    if (data.contains("entities"))
    {
        for (const auto& entityData : data["entities"])
        {
            std::string prefabId = entityData["prefab"].get<std::string>();
            
            auto prefabHandle = assets.get<Prefab>(prefabId);
            
            Entity* entity = PrefabFactory::create(prefabHandle, scene, assets);

            if (entity && entityData.contains("x") && entityData.contains("y"))
            {
                if (entity->hasComponent<TransformComponent>())
                {
                    float x = entityData["x"].get<float>();
                    float y = entityData["y"].get<float>();
                    
                    entity->getComponent<TransformComponent>()->setPosition(x, y);
                }
            }
        }
    }

    return true;
}

} // namespace dagger
