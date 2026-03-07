#ifndef DELVE_SCENES_GAMEPLAY_SCENE_H
#define DELVE_SCENES_GAMEPLAY_SCENE_H

#include <dagger/core/Entity.h>
#include <dagger/core/Scene.h>

namespace delve
{

class dagger::TransformComponent;
class dagger::TextComponent;

class GameplayScene : public dagger::Scene
{
public:
    void onInit(dagger::AssetManager& assets) override;
    void onUpdate(float dt, dagger::Application& app) override;

private:
    dagger::TransformComponent* playerTransform_ = nullptr;
    dagger::TextComponent* fpsTextComp_ = nullptr;
};

} // namespace delve

#endif // DELVE_SCENES_GAMEPLAY_SCENE_H
