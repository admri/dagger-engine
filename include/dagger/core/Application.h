#ifndef DAGGER_CORE_APPLICATION_H
#define DAGGER_CORE_APPLICATION_H

#include <dagger/core/AppConfig.h>
#include <dagger/core/GameEvent.h>
#include <dagger/core/Scene.h>
#include <dagger/core/SceneManager.h>

#include <cassert>
#include <cstdint>
#include <memory>
#include <optional>
#include <queue>

namespace dagger
{

class Window;
class Renderer;
class Input;
class AudioManager;
class AssetManager;

/// @brief The core engine class. Manages the main game loop, hardware subsystems, and global state.
/// To create a game, inherit from this class and override the lifecycle methods (onInit, onUpdate, etc.).
class Application
{
public:
    Application();
    virtual ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    /// @brief Starts the engine loop with the specified configuration. Blocks until the game is closed.
    void run(const AppConfig& config = {});

    /// @brief Signals the application to gracefully terminate at the end of the current frame.
    void quit();

    inline Window& getWindow()            { assert(window_       != nullptr && "Window accessed before Application::init()!");         return *window_; }
    inline Renderer& getRenderer()        { assert(renderer_     != nullptr && "Renderer accessed before Application::init()!");       return *renderer_; }
    inline Input& getInput()              { assert(input_        != nullptr && "Input accessed before Application::init()!");          return *input_; }
    inline AudioManager& getAudio()       { assert(audioManager_ != nullptr && "Audio manager accessed before Application::init()!");  return *audioManager_; }
    inline AssetManager& getAssets()      { assert(assets_       != nullptr && "Asset manager accessed before Application::init()!");  return *assets_; }
    inline SceneManager& getSceneManager() { return sceneManager_; }

    inline int getFPS() const { return currentFPS_; }
    int getWindowWidth() const;
    int getWindowHeight() const;

protected:
    /// @brief Called once upon application startup. Register components and initial scenes here.
    virtual bool onInit() { return true; };
    
    /// @brief Called every frame. Use for global game logic independent of the active scene.
    virtual void onUpdate(float dt) {}
    
    /// @brief Called every frame after scene rendering. Use for global overlays or debug UI.
    virtual void onRender() {}
    
    /// @brief Triggered on specific global engine events (e.g., window resized, focus lost).
    virtual void onEvent(const GameEvent& event) {}
    
    /// @brief Called right before the application is destroyed. Use for global memory cleanup.
    virtual void onShutdown() {}

private:
    bool initialized_ = false;
    bool isRunning_ = false;

    bool coreInitialized_ = false; 
    bool audioInitialized_ = false;
    bool fontInitialized_ = false;

    double fixed_dt = 1.0 / 60.0;
    uint64_t currentTime_ = 0;
    uint64_t lastTime_ = 0;
    double deltaTime_ = 0.0;
    int frameCount_ = 0;
    int currentFPS_ = 0;
    uint64_t fpsTimer_ = 0;

    std::unique_ptr<Window> window_;
    std::unique_ptr<Renderer> renderer_;
    std::unique_ptr<Input> input_;
    std::unique_ptr<AudioManager> audioManager_;
    std::unique_ptr<AssetManager> assets_;
    SceneManager sceneManager_;

    std::queue<GameEvent> eventQueue_;

    bool init(const AppConfig& config);
    void pollEvents();
    void processEvents();
    void updateFPS();
    void cleanup();
};

} // namespace dagger

#endif // DAGGER_CORE_APPLICATION_H
