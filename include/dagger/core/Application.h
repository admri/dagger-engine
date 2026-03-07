#ifndef DAGGER_CORE_APPLICATION_H
#define DAGGER_CORE_APPLICATION_H

#include <dagger/assets/AssetManager.h>
#include <dagger/audio/AudioSystem.h>
#include <dagger/core/GameEvent.h>
#include <dagger/core/Scene.h>
#include <dagger/core/Window.h>
#include <dagger/graphics/Renderer.h>
#include <dagger/input/Input.h>

#include <memory>
#include <optional>
#include <queue>
#include <stdexcept>
#include <string>

namespace dagger
{

constexpr double FIXED_DT = 1.0 / 60.0; // 60 TPS (Ticks Per Second)

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

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    void run(const AppConfig& config = {});
    void quit();

    inline Window& getWindow()             { if (window_)      return *window_;      throw std::runtime_error("Window not initialized"); }
    inline Renderer& getRenderer()         { if (renderer_)    return *renderer_;    throw std::runtime_error("Renderer not initialized"); }
    inline Input& getInput()               { if (input_)       return *input_;       throw std::runtime_error("Input not initialized"); }
    inline AudioSystem& getAudioSystem()   { if (audioSystem_) return *audioSystem_; throw std::runtime_error("Audio system not initialized"); }
    inline AssetManager& getAssets()       { if (assets_)      return *assets_;      throw std::runtime_error("Assets not initialized"); }
    
    inline int getFPS() const { return currentFPS_; }

    inline void setScene(std::unique_ptr<Scene> scene) { nextScene_ = std::move(scene); }

protected:
    virtual bool onInit() { return true; };
    virtual void onUpdate(float dt) {}
    virtual void onRender() {}
    virtual void onEvent(const GameEvent& event) {}
    virtual void onShutdown() {}

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

    std::queue<GameEvent> eventQueue_;

    bool init(const AppConfig& config);
    void pollEvents();
    void processEvents();
    void updateFPS();
    void cleanup();
};

} // namespace dagger

#endif // DAGGER_CORE_APPLICATION_H
