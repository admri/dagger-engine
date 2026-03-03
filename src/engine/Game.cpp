#include <Game.h>

#include <core/TextObject.h>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_timer.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <algorithm>

Game::~Game()
{
    quit();
}

bool Game::init(const std::string &title, int width, int height, SDL_WindowFlags flags)
{
    SDL_Log("Game initializing...");

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL: %s\n", SDL_GetError());
        return false;
    }
    SDL_Log("SDL initialized.");

    if (!TTF_Init())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL_ttf: %s\n", SDL_GetError());
        return false;
    }
    SDL_Log("SDL_TTF initialized.");

    if (!MIX_Init())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL_mixer: %s\n", SDL_GetError());
        return false;
    }
    SDL_Log("SDL_Mixer initialized.");

    window_.emplace();
    renderer_.emplace();
    input_.emplace();
    assets_.emplace(*renderer_);

    if (!assets_->initAudio())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize audio manager.\n");
        return false;
    }
    
    if (!window_->create(title, width, height, flags))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window.\n");
        return false;
    }

    if (!renderer_->create(*window_))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create renderer.\n");
        return false;
    }

    // Create scene with FPS display
    currentScene_ = std::make_unique<Scene>();

    assets_->loadFont("fpsDisplay", "assets/fonts/RobotoMono-Regular.ttf", 16);
    FontHandle fpsFontHandle = assets_->getFontHandle("fpsDisplay");

    auto fpsDisplay = std::make_unique<TextObject>();
    fpsDisplay->setFont(fpsFontHandle);
    fpsDisplay->setColor(Colors::Black);
    fpsDisplay->setPosition(10.0f, 10.0f);
    fpsDisplay->setText("FPS: 0");

    fpsDisplay_ = fpsDisplay.get();
    currentScene_->addGameObject(std::move(fpsDisplay));

    initialized_ = true;
    SDL_Log("Game initialized successfully.\n\n");
    return true;
}

void Game::run()
{
    isRunning_ = true;
    SDL_Log("Game running...\n");

    lastTime_ = SDL_GetPerformanceCounter();
    fpsTimer_ = lastTime_;

    while (isRunning_)
    {
        Uint64 currentTime = SDL_GetPerformanceCounter();
        deltaTime_ = (double)(currentTime - lastTime_) / SDL_GetPerformanceFrequency();
        lastTime_ = currentTime;

        pollEvents();
        processEvents();
        update(deltaTime_);
        render();

        frameCount_++;
        if ((currentTime - fpsTimer_) >= SDL_GetPerformanceFrequency())
        {
            currentFPS_ = frameCount_;
            frameCount_ = 0;
            fpsTimer_ = currentTime;

            if (fpsDisplay_)
            {
                fpsDisplay_->setText("FPS: " + std::to_string(currentFPS_));
            }
        }
    }

    SDL_Log("Game stopped.\n\n");
}

void Game::quit()
{
    if (!initialized_) return;

    SDL_Log("Quitting game...\n");
    isRunning_ = false;

    if (assets_) assets_->clear();
    if (renderer_) renderer_->destroy();
    if (window_) window_->destroy();
    
    assets_.reset();
    input_.reset();
    renderer_.reset();
    window_.reset();

    SDL_Log("Resources cleaned up.\n");

    TTF_Quit();
    MIX_Quit();
    SDL_Log("Quitting SDL...");
    SDL_Quit();
    
    initialized_ = false;
}

void Game::pollEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_EVENT_QUIT:
                eventQueue_.push({GameEventType::Quit});
                break;
        }
    }

    if (input_->isKeyPressed(SDL_SCANCODE_ESCAPE))
    {
        eventQueue_.push({GameEventType::Quit});
    }
    if (input_->isKeyPressed(SDL_SCANCODE_V))
    {
        eventQueue_.push({GameEventType::ToggleVSync});
    }
}

void Game::processEvents()
{
    while (!eventQueue_.empty() && isRunning_)
    {
        const GameEvent& event = eventQueue_.front();

        switch (event.type)
        {
            case GameEventType::ToggleVSync:
                renderer_->toggleVSync();
                break;

            case GameEventType::Quit:
                isRunning_ = false;
                break;
        }

        eventQueue_.pop();
    }
}

void Game::update(float dt)
{
    input_->update();

    if (currentScene_)
    {
        currentScene_->update(dt);
    }
}

void Game::render()
{
    renderer_->clear(Colors::White);

    if (currentScene_)
    {
        currentScene_->render(*renderer_, *assets_);
    }

    renderer_->present();
}
