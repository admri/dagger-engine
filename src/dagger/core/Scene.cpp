#include <core/Scene.h>

#include <assets/AssetManager.h>
#include <graphics/Renderer.h>
#include <input/Input.h>

namespace dagger
{

void Scene::update(float dt, Input& input)
{
    for (auto& obj : gameObjects_)
    {
        if (obj->isActive())
        {
            obj->update(dt, input);
        }
    }

    // Erase inactive game objects
    gameObjects_.erase(
        std::remove_if(
            gameObjects_.begin(),
            gameObjects_.end(),
            [](auto& obj) { return !obj->isActive(); }),
        gameObjects_.end()
    );
}

void Scene::render(Renderer& renderer, AssetManager& assets)
{
    for (auto& obj : gameObjects_)
    {
        if (obj->isActive())
        {
            obj->render(renderer, assets);
        }
    }
}

void Scene::addGameObject(std::unique_ptr<GameObject> obj)
{
    gameObjects_.push_back(std::move(obj));
}

} // namespace dagger
