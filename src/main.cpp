#include <engine/Game.h>

#include <SDL3/SDL_main.h>

#include <iostream>

int main(int argc, char** argv)
{
    Game game;

    if (!game.init("Dungeon Crawler", 800, 600))
    {
        std::cerr << "Failed to initialize the game." << std::endl;
        return -1;
    }

    game.run();

    return 0;
}
