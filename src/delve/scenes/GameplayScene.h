#ifndef DELVE_SCENES_GAMEPLAY_SCENE_H
#define DELVE_SCENES_GAMEPLAY_SCENE_H

#include <systems/PlayerInputSystem.h>

#include <dagger/core/Entity.h>
#include <dagger/core/Scene.h>
#include <dagger/core/systems/MovementSystem.h>

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

private:
    dagger::Entity* camera_ = nullptr;
    dagger::Entity* player_ = nullptr;
    dagger::Entity* fpsDisplay_ = nullptr;

    PlayerInputSystem inputSystem_;
    dagger::MovementSystem movementSystem_;
};

} // namespace delve

#endif // DELVE_SCENES_GAMEPLAY_SCENE_H
