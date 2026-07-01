#include <core/Application.h>

#include <assets/AssetManager.h>
#include <assets/PrefabFactory.h>
#include <audio/AudioManager.h>
#include <core/Constants.h>
#include <core/Window.h>
#include <graphics/Renderer.h>
#include <input/Input.h>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_timer.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

#ifdef __APPLE__
#include <graphics/MacHacks.h>
#endif

namespace dagger
{

Application::Application()
{
}

Application::~Application()
{
    if (initialized_)
    {
        cleanup();
    }
}

bool Application::init(const AppConfig& config)
{
    if (initialized_) return true;

    SDL_Log("Application initializing...");

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL: %s\n", SDL_GetError());
        quit();
        return false;
    }
    coreInitialized_ = true;
    SDL_Log("SDL initialized.");

    if (!TTF_Init())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL_ttf: %s\n", SDL_GetError());
        quit();
        return false;
    }
    fontInitialized_ = true;
    SDL_Log("SDL_TTF initialized.");

    if (!MIX_Init())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL_mixer: %s\n", SDL_GetError());
        quit();
        return false;
    }
    audioInitialized_ = true;
    SDL_Log("SDL_Mixer initialized.");

    window_ = std::make_unique<Window>();
    renderer_ = std::make_unique<Renderer>();
    input_ = std::make_unique<Input>();
    audioManager_ = std::make_unique<AudioManager>();
    assets_ = std::make_unique<AssetManager>(*renderer_, *audioManager_);

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
#ifdef __APPLE__
    //ApplyMetalOptimizations(renderer_->get());
#endif

    input_->init();

    if (!audioManager_->init())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize audio manager.\n");
        quit();
        return false;
    }

    if (config.vsync) renderer_->toggleVSync();
    fixed_dt = 1.0 / config.tps;

    PrefabFactory::registerCoreComponents();

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

    double accumulator = 0.0;
    while (isRunning_)
    {
        // Timing
        currentTime_ = SDL_GetPerformanceCounter();
        deltaTime_ = (double)(currentTime_ - lastTime_) / SDL_GetPerformanceFrequency();
        lastTime_ = currentTime_;

        if (deltaTime_ > 0.25) deltaTime_ = 0.25;
        accumulator += deltaTime_;

        // Events
        input_->update();
        pollEvents();
        processEvents();

        // Change scene
        sceneManager_.processActions(*this);

        if (sceneManager_.isEmpty())
        {
            isRunning_ = false;
            continue;
        }

        // Update
        while (accumulator >= fixed_dt)
        {
            sceneManager_.fixedUpdate(static_cast<float>(fixed_dt), *this);
            accumulator -= fixed_dt;
        }
        
        float alpha = static_cast<float>(accumulator / fixed_dt);

        sceneManager_.update(static_cast<float>(deltaTime_), *this);
        onUpdate(static_cast<float>(deltaTime_));

        // Render
        renderer_->clear(Colors::Black);
        
        sceneManager_.render(*renderer_, *assets_, alpha);
        onRender();

        renderer_->present();
        updateFPS();
    }

    cleanup();
}

void Application::quit()
{
    isRunning_ = false;
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
    }
    frameCount_++;
}

int Application::getWindowWidth() const
{
    return window_->getWidth();
}

int Application::getWindowHeight() const
{
    return window_->getHeight();
}

void Application::cleanup()
{
    SDL_Log("Shutting down application...");
    onShutdown();

    SDL_Log("Cleaning up resources...");
    if (assets_) assets_->clear();
    if (renderer_) renderer_->destroy();
    if (window_) window_->destroy();
    
    sceneManager_.clear();
    assets_.reset();
    audioManager_.reset();
    input_.reset();
    renderer_.reset();
    window_.reset();
    SDL_Log("Resources cleaned up.");

    if (audioInitialized_)
    {
        SDL_Log("Quitting SDL_mixer...");
        MIX_Quit();
        audioInitialized_ = false;
    }
    if (fontInitialized_)
    {
        SDL_Log("Quitting SDL_ttf...");
        TTF_Quit();
        fontInitialized_ = false;
    }
    if (coreInitialized_)
    {
        SDL_Log("Quitting SDL...");
        SDL_Quit();
        coreInitialized_ = false;
    }

    SDL_Log("Application shutdown complete.");
    initialized_ = false;
}

} // namespace dagger
