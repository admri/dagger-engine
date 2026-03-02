#ifndef GAME_H
#define GAME_H

#include <assets/AssetManager.h>
#include <core/GameEvent.h>
#include <core/GameObject.h>
#include <core/TextObject.h>
#include <core/Window.h>
#include <graphics/Renderer.h>
#include <input/Input.h>

#include <SDL3/SDL_video.h>

#include <memory>
#include <optional>
#include <queue>
#include <vector>

class Game
{
public:
    Game()
        : isRunning_(false) {}
    ~Game();

    bool init(const std::string& title, int width, int height, SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE);
    void run();
    void quit();

private:
    bool initialized_ = false;
    bool isRunning_;

    Uint64 lastTime_ = 0;
    double deltaTime_ = 0.0;
    int frameCount_ = 0;
    int currentFPS_ = 0;
    Uint64 fpsTimer_ = 0;

    std::optional<Window> window_;
    std::optional<Renderer> renderer_;
    std::optional<Input> input_;
    std::optional<AssetManager> assets_;

    std::queue<GameEvent> eventQueue_;
    std::vector<std::unique_ptr<GameObject>> gameObjects_;
    TextObject* fpsDisplay_;

    void pollEvents();
    void processEvents();
    void update(float dt);
    void render();
};

#endif // GAME_H
