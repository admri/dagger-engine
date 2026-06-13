#ifndef DAGGER_CORE_SCENE_MANAGER_H
#define DAGGER_CORE_SCENE_MANAGER_H

#include <dagger/core/Scene.h>

#include <memory>
#include <vector>

namespace dagger
{

class Application;
class Renderer;
class AssetManager;

class SceneManager
{
public:
    enum class ActionType
    {
        Push,
        Pop,
        Replace
    };

    struct PendingAction
    {
        ActionType action;
        std::unique_ptr<Scene> scene;
    };

    void push(std::unique_ptr<Scene> scene);
    void pop();
    void replace(std::unique_ptr<Scene> scene);

    void processActions(Application& app);

    bool isEmpty() const { return sceneStack_.empty(); }

    void fixedUpdate(float dt, Application& app);
    void update(float dt, Application& app);
    void render(Renderer& renderer, AssetManager& assets, float alpha);

    void clear();

private:
    std::vector<std::unique_ptr<Scene>> sceneStack_;
    std::vector<PendingAction> pendingActions_;
};

} // namespace dagger

#endif // DAGGER_CORE_SCENE_MANAGER_H
