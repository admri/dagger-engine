#ifndef SCENE_H
#define SCENE_H

#include <engine/core/GameObject.h>

#include <memory>
#include <vector>

namespace engine
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

} // namespace engine

#endif // SCENE_H
