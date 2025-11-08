#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>

int main(int argc, char* argv[])
{
    SDL_Log("Initializing SDL...\n");

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Log("Creating window...\n");

    SDL_Window* window = SDL_CreateWindow("Dungeon Crawler", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!window)
	{
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Log("Starting Dungeon Crawler...\n");

    SDL_Event event;
    bool running = true;

    while (running)
    {
        while (SDL_PollEvent(&event))
		{
            if (event.type == SDL_EVENT_QUIT)
			{
                running = false;
			}
        }
        SDL_Delay(16);
    }

    SDL_Log("Quitting SDL...\n");

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
