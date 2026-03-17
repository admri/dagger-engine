#include <Game.h>

#include <dagger/core/AppConfig.h>

#include <SDL3/SDL_main.h>

#include <iostream>

int main(int argc, char** argv)
{
    delve::Game game;

    try
    {
        game.run({"Dungeon Crawler", 800, 600, dagger::WindowFlags::Resizable, true, true});
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "An unknown error occurred.\n";
    }

    return 0;
}
