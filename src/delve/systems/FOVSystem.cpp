#include <systems/FOVSystem.h>

#include <components/FOVComponent.h>
#include <components/PlayerInputComponent.h>
#include <world/TileMap.h>

#include <dagger/core/Scene.h>
#include <dagger/core/components/SpriteComponent.h>
#include <dagger/core/components/Tags.h>
#include <dagger/core/components/TransformComponent.h>

#include <cstdint>
#include <numbers>

namespace delve
{

void FOVSystem::update(float dt, dagger::Scene& scene, TileMap& tileMap)
{
    FOVComponent* activeFov = nullptr;

    scene.forEachEntityWith<dagger::TransformComponent, FOVComponent>(
        [&](auto* entity, auto& transform, auto& fov) 
    {
        activeFov = &fov;
        fov.visibleTiles.clear();

        for (int i = 0; i < fov.numRays; ++i)
        {
            float angle = (i * 360.0f / fov.numRays) * (std::numbers::pi_v<float> / 180.0f);
            float dx = std::cos(angle);
            float dy = std::sin(angle);
            
            for (float currentDist = 0; currentDist < fov.radius; currentDist += fov.stepSize)
            {
                float rayX = transform.x + dx * currentDist;
                float rayY = transform.y + dy * currentDist;
                
                int tileX = static_cast<int>(rayX / tileMap.getTileSize());
                int tileY = static_cast<int>(rayY / tileMap.getTileSize());

                uint64_t tileKey = (static_cast<uint64_t>(tileX) << 32) | tileY;
                
                fov.visibleTiles.insert(tileKey);
                fov.exploredTiles.insert(tileKey);
                
                if (tileMap.getTile(tileX, tileY) == TileType::Wall)
                {
                    break;
                }
            }
        }
    });

    if (!activeFov) return;

    scene.forEachEntityWith<dagger::TransformComponent, dagger::SpriteComponent>(
        [&](dagger::Entity* e, auto& transform, auto& sprite)
    {
        if (e->hasComponent<PlayerInputComponent>()) return;

        int tileX = static_cast<int>(transform.x / tileMap.getTileSize());
        int tileY = static_cast<int>(transform.y / tileMap.getTileSize());
        uint64_t tileKey = (static_cast<uint64_t>(tileX) << 32) | tileY;

        bool isVisible = activeFov->visibleTiles.count(tileKey) > 0;
        bool isExplored = activeFov->exploredTiles.count(tileKey) > 0;

        if (!isVisible && !isExplored)
        {
            // Hidden
            if (!e->hasComponent<dagger::HiddenTag>()) e->addComponent<dagger::HiddenTag>();
            if (e->hasComponent<dagger::ShadowTag>()) e->removeComponent<dagger::ShadowTag>();
        }
        else if (!isVisible && isExplored)
        {
            // Shadow
            if (e->hasComponent<dagger::HiddenTag>()) e->removeComponent<dagger::HiddenTag>();
            if (!e->hasComponent<dagger::ShadowTag>()) e->addComponent<dagger::ShadowTag>();
        }
        else
        {
            // Visible
            if (e->hasComponent<dagger::HiddenTag>()) e->removeComponent<dagger::HiddenTag>();
            if (e->hasComponent<dagger::ShadowTag>()) e->removeComponent<dagger::ShadowTag>();
        }
    });

}

} // namespace delve
