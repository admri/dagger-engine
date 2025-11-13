#include "Game.h"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_timer.h>

#include <iostream>

Game::Game()
    : isRunning_(false)
{
}

bool Game::init(const std::string& title, int width, int height, SDL_WindowFlags flags)
{
    SDL_Log("Initializing game...");

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL: %s\n", SDL_GetError());
        return false;
    }
    
    if (!window_.create(title, width, height, flags))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window.\n");
        return false;
    }

    if (!renderer_.create(window_))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create renderer.\n");
        return false;
    }

    SDL_Log("Game initialized successfully.\n");
    return true;
}

void Game::run()
{
    isRunning_ = true;
    SDL_Log("Game running.\n");

    Uint64 lastTicks = SDL_GetTicks();

    while (isRunning_)
    {
        Uint64 currentTicks = SDL_GetTicks();
        float deltaTime = (currentTicks - lastTicks) / 1000.0f;
        lastTicks = currentTicks;

        pollEvents();
        processEvents();

        update(deltaTime);
        render();
    }

    SDL_Log("Game stopped.\n");
}

void Game::quit()
{
    isRunning_ = false;
    renderer_.destroy();
    window_.destroy();
    SDL_Quit();
}

void Game::pollEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            eventQueue_.push({GameEventType::Quit});
        }
    }

    if (input_.isKeyPressed(SDL_SCANCODE_ESCAPE))
    {
        eventQueue_.push({GameEventType::Quit});
    }
}

void Game::processEvents()
{
    while (!eventQueue_.empty())
    {
        const GameEvent& event = eventQueue_.front();

        switch (event.type)
        {
            case GameEventType::Quit:
                quit();
                break;
        }

        eventQueue_.pop();
    }
}

void Game::update(float dt)
{
    input_.update();
}

void Game::render()
{
    renderer_.clear(Colors::White);
    renderer_.present();
}