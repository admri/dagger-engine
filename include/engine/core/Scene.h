#ifndef SCENE_H
#define SCENE_H

#include <core/GameObject.h>

#include <memory>
#include <vector>

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

#endif // SCENE_H
