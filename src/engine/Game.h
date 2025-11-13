#ifndef GAME_H
#define GAME_H

#include "GameEvent.h"
#include "Input.h"
#include "Renderer.h"
#include "Window.h"

#include <SDL3/SDL_video.h>

#include <queue>

class Game
{
public:
    Game();

    bool init(const std::string& title, int width, int height, SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE);
    void run();
    void quit();

private:
    bool isRunning_;
    std::queue<GameEvent> eventQueue_;

    Window window_;
    Renderer renderer_;
    Input input_;

    void pollEvents();
    void processEvents();
    void update(float dt);
    void render();
};

#endif // GAME_H