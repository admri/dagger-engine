#include <delve/Game.h>

#include <SDL3/SDL_main.h>

#include <iostream>

int main(int argc, char** argv)
{
    delve::Game game;

    game.run({"Dungeon Crawler", 800, 600, SDL_WINDOW_RESIZABLE, false, true});

    return 0;
}
