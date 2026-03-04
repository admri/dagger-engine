#ifndef DAGGER_CORE_APPLICATION_H
#define DAGGER_CORE_APPLICATION_H

#include <dagger/assets/AssetManager.h>
#include <dagger/audio/AudioSystem.h>
#include <dagger/core/DebugScene.h>
#include <dagger/core/GameEvent.h>
#include <dagger/core/Scene.h>
#include <dagger/core/Window.h>
#include <dagger/graphics/Renderer.h>
#include <dagger/input/Input.h>

#include <memory>
#include <optional>
#include <queue>
#include <string>

namespace dagger
{

struct AppConfig
{
    std::string title = "Engine Game";
    int width = 800;
    int height = 600;
    SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE;
    bool showFPS = true;
    bool vsync = true;
};

class Application
{
public:
    Application() : isRunning_(false) {}
    virtual ~Application();

    void run(const AppConfig& config = {});
    void quit();

protected:
    virtual bool onInit() { return true; };
    virtual void onUpdate(float dt) {}
    virtual void onRender() {}
    virtual void onEvent(const GameEvent& event) {}
    virtual void onShutdown() {}

    Window& getWindow()             { return *window_; }
    Renderer& getRenderer()         { return *renderer_; }
    Input& getInput()               { return *input_; }
    AudioSystem& getAudioSystem()   { return *audioSystem_; }
    AssetManager& getAssets()       { return *assets_; }
    
    int getFPS() const              { return currentFPS_; }

    void setScene(std::unique_ptr<Scene> scene)
    {
        nextScene_ = std::move(scene);
    }

    std::queue<GameEvent> eventQueue_;

private:
    bool initialized_ = false;
    bool sdlInit_ = false;
    bool ttfInit_ = false;
    bool mixInit_ = false;
    bool isRunning_;

    Uint64 currentTime_ = 0;
    Uint64 lastTime_ = 0;
    double deltaTime_ = 0.0;
    int frameCount_ = 0;
    int currentFPS_ = 0;
    Uint64 fpsTimer_ = 0;

    std::optional<Window> window_;
    std::optional<Renderer> renderer_;
    std::optional<Input> input_;
    std::optional<AudioSystem> audioSystem_;
    std::optional<AssetManager> assets_;

    std::unique_ptr<Scene> currentScene_;
    std::unique_ptr<Scene> nextScene_;
    std::unique_ptr<DebugScene> debugScene_;

    bool init(const AppConfig& config);
    void pollEvents();
    void processEvents();
    void updateFPS();
};

} // namespace dagger

#endif // DAGGER_CORE_APPLICATION_H
