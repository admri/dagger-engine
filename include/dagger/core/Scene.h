#ifndef DAGGER_CORE_SCENE_H
#define DAGGER_CORE_SCENE_H

#include <dagger/core/GameObject.h>

#include <memory>
#include <vector>

namespace dagger
{

class Renderer;
class AssetManager;

class Scene
{
public:
    virtual ~Scene() = default;

    virtual void init(AssetManager& assets) {}

    virtual void update(float dt);
    virtual void render(Renderer& renderer, AssetManager& assets);

    void addGameObject(std::unique_ptr<GameObject> obj);

protected:
    std::vector<std::unique_ptr<GameObject>> gameObjects_;
};

} // namespace dagger

#endif // DAGGER_CORE_SCENE_H
