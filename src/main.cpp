#include <Game.h>

#include <dagger/core/AppConfig.h>

#include <iostream>
#include <stdexcept>

int main(int argc, char** argv)
{
    delve::Game game;

    dagger::AppConfig config {
        "Dungeon Crawler",
        800, 600,
        dagger::WindowFlags::Resizable,
        true,  // Show FPS
        false  // Turn on V-Sync
    };

    try
    {
        game.run(config);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal Error: " << e.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "An unknown error occurred.\n";
    }

    return 0;
}
