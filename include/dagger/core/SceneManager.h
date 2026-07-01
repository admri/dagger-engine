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

/// @brief Manages a stack of active game scenes (State Machine).
/// Handles safe transitions between scenes without crashing iterating systems.
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

    /// @brief Pushes a new scene onto the stack, pausing the current one.
    void push(std::unique_ptr<Scene> scene);

    /// @brief Pops the top scene off the stack, resuming the previous one.
    void pop();

    /// @brief Swaps the current scene with a new one (destroys the current scene).
    void replace(std::unique_ptr<Scene> scene);

    /// @brief Processes any queued scene transitions safely outside the update loop.
    void processActions(Application& app);

    bool isEmpty() const { return sceneStack_.empty(); }

    void fixedUpdate(float dt, Application& app);
    void update(float dt, Application& app);
    void render(Renderer& renderer, AssetManager& assets, float alpha);

    /// @brief Destroys all scenes currently in the stack.
    void clear();

private:
    std::vector<std::unique_ptr<Scene>> sceneStack_;
    std::vector<PendingAction> pendingActions_;
};

} // namespace dagger

#endif // DAGGER_CORE_SCENE_MANAGER_H
