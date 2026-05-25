#ifndef DELVE_SCENES_GAMEPLAY_SCENE_H
#define DELVE_SCENES_GAMEPLAY_SCENE_H

#include <systems/MovementSystem.h>
#include <systems/PlayerInputSystem.h>
#include <world/TileMap.h>

#include <dagger/core/Entity.h>
#include <dagger/core/Scene.h>

#include <memory>

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
    void onInit(dagger::AssetManager& assets) override;
    void onFixedUpdate(float dt, dagger::Application& app) override;
    void onUpdate(float dt, dagger::Application& app) override;
    void onRender(dagger::Renderer& renderer, dagger::AssetManager& assets, float alpha) override;

private:
    dagger::Entity* camera_ = nullptr;
    dagger::Entity* player_ = nullptr;
    dagger::Entity* fpsDisplay_ = nullptr;

    PlayerInputSystem inputSystem_;
    MovementSystem movementSystem_;

    std::unique_ptr<TileMap> tileMap_;
    dagger::TextureHandle floorTex_;
    dagger::TextureHandle wallTex_;
};

} // namespace delve

#endif // DELVE_SCENES_GAMEPLAY_SCENE_H
