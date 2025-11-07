#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>

int main(int argc, char* argv[])
{
    std::cout << "Welcome to Dungeon Crawler!" << std::endl;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Dungeon Crawler", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!window)
	{
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Event event;
    bool running = true;
    Uint64 start = SDL_GetTicks();

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

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
