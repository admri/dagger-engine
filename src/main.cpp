#include <dagger/main.h>
#include <Game.h>

#include <iostream>
#include <stdexcept>

#include <SDL3/SDL_log.h>

int main(int argc, char* argv[])
{
    delve::Game game;

    dagger::AppConfig config {
        .title = "Dungeon Crawler",
        .width = 800,
        .height = 600,
        .flags = dagger::WindowFlags::Resizable,
        .showFPS = true,
        .vsync = true
    };

    try
    {
        game.run(config);
    }
    catch (const std::exception& e)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Fatal Error: ", e.what());
    }
    catch (...)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "An unknown error occurred.");
    }

    return 0;
}
