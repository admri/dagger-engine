#include <dagger/main.h>
#include <Game.h>

#include <iostream>
#include <stdexcept>

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
        std::cerr << "Fatal Error: " << e.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "An unknown error occurred.\n";
    }

    return 0;
}
