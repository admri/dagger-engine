#ifndef DAGGER_CORE_SCENE_H
#define DAGGER_CORE_SCENE_H

#include <dagger/core/Entity.h>
#include <dagger/core/systems/RenderSystem.h>

#include <memory>
#include <vector>

namespace dagger
{

class Application;
class Renderer;
class AssetManager;
class Input;

class Scene
{
public:
    virtual ~Scene() = default;

    virtual void onInit(AssetManager& assets) = 0;
    virtual void onFixedUpdate(float dt, Application& app) {}
    virtual void onUpdate(float dt, Application& app) {}
    virtual void onRender(Renderer& renderer, AssetManager& assets, float alpha = 0.0f)
    {
        renderSystem_.render(renderer, assets, alpha);
    }

    Entity* createEntity()
    {
        auto entity = std::make_unique<Entity>();
        Entity* ptr = entity.get();
        entities_.push_back(std::move(entity));
        return ptr;
    }

protected:
    std::vector<std::unique_ptr<Entity>> entities_;
    RenderSystem renderSystem_;
};

} // namespace dagger

#endif // DAGGER_CORE_SCENE_H
