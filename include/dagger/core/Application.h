#ifndef DAGGER_CORE_APPLICATION_H
#define DAGGER_CORE_APPLICATION_H

#include <dagger/core/AppConfig.h>
#include <dagger/core/GameEvent.h>
#include <dagger/core/Scene.h>

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
class AudioSystem;
class AssetManager;

class Application
{
public:
    Application();
    virtual ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    void run(const AppConfig& config = {});
    void quit();

    inline Window& getWindow()           { assert(window_      != nullptr && "Window accessed before Application::init()!");        return *window_; }
    inline Renderer& getRenderer()       { assert(renderer_    != nullptr && "Renderer accessed before Application::init()!");      return *renderer_; }
    inline Input& getInput()             { assert(input_       != nullptr && "Input accessed before Application::init()!");         return *input_; }
    inline AudioSystem& getAudioSystem() { assert(audioSystem_ != nullptr && "Audio system accessed before Application::init()!");  return *audioSystem_; }
    inline AssetManager& getAssets()     { assert(assets_      != nullptr && "Asset manager accessed before Application::init()!"); return *assets_; }
    
    inline int getFPS() const { return currentFPS_; }
    int getWindowWidth() const;
    int getWindowHeight() const;

    inline void setScene(std::unique_ptr<Scene> scene) { nextScene_ = std::move(scene); }

protected:
    virtual bool onInit() { return true; };
    virtual void onUpdate(float dt) {}
    virtual void onRender() {}
    virtual void onEvent(const GameEvent& event) {}
    virtual void onShutdown() {}

private:
    bool initialized_ = false;
    bool isRunning_ = false;

    bool coreInitialized_ = false; 
    bool audioInitialized_ = false;
    bool fontInitialized_ = false;

    uint64_t currentTime_ = 0;
    uint64_t lastTime_ = 0;
    double deltaTime_ = 0.0;
    int frameCount_ = 0;
    int currentFPS_ = 0;
    uint64_t fpsTimer_ = 0;

    std::unique_ptr<Window> window_;
    std::unique_ptr<Renderer> renderer_;
    std::unique_ptr<Input> input_;
    std::unique_ptr<AudioSystem> audioSystem_;
    std::unique_ptr<AssetManager> assets_;

    std::unique_ptr<Scene> currentScene_;
    std::unique_ptr<Scene> nextScene_;

    std::queue<GameEvent> eventQueue_;

    bool init(const AppConfig& config);
    void pollEvents();
    void processEvents();
    void updateFPS();
    void cleanup();
};

} // namespace dagger

#endif // DAGGER_CORE_APPLICATION_H
