#include <core/SceneManager.h>

#include <dagger/assets/AssetManager.h>
#include <dagger/core/Application.h>
#include <dagger/core/Scene.h>
#include <dagger/graphics/Renderer.h>

namespace dagger
{

void SceneManager::push(std::unique_ptr<Scene> scene)
{
    pendingActions_.push_back({SceneManager::ActionType::Push, std::move(scene)});
}

void SceneManager::pop()
{
    pendingActions_.push_back({SceneManager::ActionType::Pop, nullptr});
}

void SceneManager::replace(std::unique_ptr<Scene> scene)
{
    pendingActions_.push_back({SceneManager::ActionType::Replace, std::move(scene)});
}

void SceneManager::processActions(Application& app)
{
    for (auto& action : pendingActions_)
    {
        switch (action.action)
        {
            case SceneManager::ActionType::Push:
                sceneStack_.push_back(std::move(action.scene));
                sceneStack_.back()->onInit(app);
                break;

            case SceneManager::ActionType::Pop:
                if (!sceneStack_.empty())
                {
                    sceneStack_.pop_back();
                }
                break;

            case SceneManager::ActionType::Replace:
                sceneStack_.clear();
                sceneStack_.push_back(std::move(action.scene));
                sceneStack_.back()->onInit(app);
                break;
        }
    }
    pendingActions_.clear();
}

void SceneManager::fixedUpdate(float dt, Application& app)
{
    // Update only the top scene
    if (!sceneStack_.empty())
    {
        sceneStack_.back()->onFixedUpdate(dt, app);
    }
}

void SceneManager::update(float dt, Application& app)
{
    // Update only the top scene
    if (!sceneStack_.empty())
    {
        sceneStack_.back()->onUpdate(dt, app);
    
        // Cleanup dead entities and activate pending entities
        sceneStack_.back()->cleanup();
    }
}

void SceneManager::render(Renderer& renderer, AssetManager& assets, float alpha)
{
    // Render all scenes
    for (size_t i = 0; i < sceneStack_.size(); ++i)
    {
        float sceneAlpha = (i == sceneStack_.size() - 1) ? alpha : 1.0f; // Interpolate only the top scene
        sceneStack_[i]->onRender(renderer, assets, sceneAlpha);
    }
}

void SceneManager::clear()
{
    sceneStack_.clear();
    pendingActions_.clear();
}

} // namespace dagger
