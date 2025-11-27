#ifndef GAME_H
#define GAME_H

#include <assets/AssetManager.h>
#include <core/GameEvent.h>
#include <core/Window.h>
#include <graphics/Renderer.h>
#include <input/Input.h>

#include <SDL3/SDL_video.h>

#include <memory>
#include <queue>

class Game
{
public:
    Game() :
        isRunning_(false),
        assets_(renderer_) {}
    ~Game() { quit(); }

    bool init(const std::string& title, int width, int height, SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE);
    void run();
    void quit();

private:
    bool isRunning_;
    std::queue<GameEvent> eventQueue_;

    Uint64 lastTime_ = 0;
    double deltaTime_ = 0.0;
    int frameCount_ = 0;
    int currentFPS_ = 0;
    Uint64 fpsTimer_ = 0;

    Window window_;
    Renderer renderer_;
    Input input_;
    AssetManager assets_;

    void pollEvents();
    void processEvents();
    void update(float dt);
    void render();
};

#endif // GAME_H