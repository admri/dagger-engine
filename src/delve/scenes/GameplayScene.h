#ifndef DELVE_SCENES_GAMEPLAY_SCENE_H
#define DELVE_SCENES_GAMEPLAY_SCENE_H

#include <systems/CombatSystem.h>
#include <systems/EnemyAISystem.h>
#include <systems/FOVSystem.h>
#include <systems/MovementSystem.h>
#include <systems/PlayerInputSystem.h>
#include <systems/ShootingSystem.h>
#include <systems/TileMapRenderSystem.h>
#include <systems/UISystem.h>
#include <world/TileMap.h>

#include <dagger/core/Scene.h>
#include <dagger/core/systems/AnimationSystem.h>
#include <dagger/core/systems/CameraSystem.h>
#include <dagger/core/systems/CollisionSystem.h>
#include <dagger/core/systems/SpriteRenderSystem.h>
#include <dagger/core/systems/UIRenderSystem.h>

#include <memory>
#include <vector>

namespace dagger
{
    class AssetManager;
    class Application;
}

namespace delve
{

class GameplayScene : public dagger::Scene
{
public:
    void onInit(dagger::Application& app) override;
    void onFixedUpdate(float dt, dagger::Application& app) override;
    void onUpdate(float dt, dagger::Application& app) override;
    void onRender(dagger::Renderer& renderer, dagger::AssetManager& assets, float alpha) override;

private:
    dagger::CameraSystem cameraSystem_;
    dagger::AnimationSystem animationSystem_;
    UISystem uiSystem_;
    PlayerInputSystem inputSystem_;
    MovementSystem movementSystem_;
    CombatSystem combatSystem_;
    EnemyAISystem enemyAISystem_;
    ShootingSystem shootingSystem_;
    FOVSystem fovSystem_;
    dagger::SpriteRenderSystem spriteRenderSystem_;
    dagger::UIRenderSystem uiRenderSystem_;
    TileMapRenderSystem tileMapRenderSystem_;
    
    std::unique_ptr<TileMap> tileMap_;
    dagger::TextureHandle floorTex_;
    dagger::TextureHandle wallTex_;

    std::vector<dagger::CollisionEvent> frameCollisions_;
};

} // namespace delve

#endif // DELVE_SCENES_GAMEPLAY_SCENE_H
