#include <core/Application.h>

#include <core/SpriteObject.h>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_timer.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

namespace engine
{

Application::~Application()
{
    quit();
}

bool Application::init(const AppConfig& config)
{
    SDL_Log("Application initializing...");

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL: %s\n", SDL_GetError());
        quit();
        return false;
    }
    sdlInit_ = true;
    SDL_Log("SDL initialized.");

    if (!TTF_Init())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL_ttf: %s\n", SDL_GetError());
        quit();
        return false;
    }
    ttfInit_ = true;
    SDL_Log("SDL_TTF initialized.");

    if (!MIX_Init())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL_mixer: %s\n", SDL_GetError());
        quit();
        return false;
    }
    mixInit_ = true;
    SDL_Log("SDL_Mixer initialized.");

    window_.emplace();
    renderer_.emplace();
    input_.emplace();
    audioSystem_.emplace();
    assets_.emplace(*renderer_, *audioSystem_);

    if (!window_->create(config.title, config.width, config.height, config.flags))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window.\n");
        quit();
        return false;
    }

    if (!renderer_->create(*window_))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create renderer.\n");
        quit();
        return false;
    }

    input_->init();

    if (!audioSystem_->init())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize audio system.\n");
        quit();
        return false;
    }

    if (config.showFPS)
    {
        debugScene_ = std::make_unique<DebugScene>();
        debugScene_->init(*assets_);
    }

    if (!config.vsync) renderer_->toggleVSync();

    initialized_ = true;

    if (!onInit()) return false;

    return true;
}

void Application::run(const AppConfig& config)
{
    if (!init(config)) return;

    isRunning_ = true;
    lastTime_ = SDL_GetPerformanceCounter();
    fpsTimer_ = lastTime_;

    while (isRunning_)
    {
        // Timing
        currentTime_ = SDL_GetPerformanceCounter();
        deltaTime_ = (double)(currentTime_ - lastTime_) / SDL_GetPerformanceFrequency();
        lastTime_ = currentTime_;

        // Events
        input_->update();
        pollEvents();
        processEvents();

        // Change scene
        if (nextScene_)
        {
            currentScene_ = std::move(nextScene_);
            currentScene_->init(*assets_);
        }

        // Update
        if (currentScene_) currentScene_->update(static_cast<float>(deltaTime_));
        if (debugScene_) debugScene_->update(static_cast<float>(deltaTime_));
        onUpdate(static_cast<float>(deltaTime_));

        // Render
        renderer_->clear(Colors::Black);
        
        if (currentScene_) currentScene_->render(*renderer_, *assets_);
        if (debugScene_) debugScene_->render(*renderer_, *assets_);
        onRender();

        renderer_->present();
        updateFPS();
    }
}

void Application::quit()
{
    isRunning_ = false;
    onShutdown();

    if (assets_) assets_->clear();
    if (renderer_) renderer_->destroy();
    if (window_) window_->destroy();
    
    currentScene_.reset();
    debugScene_.reset();
    assets_.reset();
    audioSystem_.reset();
    input_.reset();
    renderer_.reset();
    window_.reset();

    if (mixInit_)
    {
        MIX_Quit();
        mixInit_ = false;
    }
    if (ttfInit_)
    {
        TTF_Quit();
        ttfInit_ = false;
    }
    if (sdlInit_)
    {
        SDL_Quit();
        sdlInit_ = false;
    }
    
    initialized_ = false;
}

void Application::pollEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            eventQueue_.push({GameEventType::Quit});
        }
    }
}

void Application::processEvents()
{
    while (!eventQueue_.empty() && isRunning_)
    {
        const GameEvent& event = eventQueue_.front();
        
        switch (event.type)
        {
            case GameEventType::Quit:
                isRunning_ = false;
                break;

            default:
                onEvent(event);
                break;
        }

        eventQueue_.pop();
    }
}

void Application::updateFPS()
{
    if ((currentTime_ - fpsTimer_) >= SDL_GetPerformanceFrequency())
    {
        currentFPS_ = frameCount_;
        frameCount_ = 0;
        fpsTimer_ = currentTime_;
        
        if (debugScene_) debugScene_->updateFPS(currentFPS_);
    }
    frameCount_++;
}

} // namespace engine
