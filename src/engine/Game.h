#pragma once

#include "Input.h"
#include "Renderer.h"
#include "Window.h"

#include <SDL3/SDL_video.h>

class Game
{
public:
    Game();

    bool init(const std::string& title, int width, int height, SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE);
    void run();
    void quit();

private:
    bool isRunning_;

    Window window_;
    Renderer renderer_;
    Input input_;

    void handleEvents();
    void update(float dt);
    void render();
};