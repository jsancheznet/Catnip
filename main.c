#include <stdio.h>
#include <SDL.h>

#include "typedefs.h"

global b32 Running = 1;

void HandleEvents()
{
    SDL_Event Event;
    while(SDL_PollEvent(&Event))
    {
        switch(Event.type)
        {
            case SDL_QUIT:
            {
                Running = 0;
                break;
            }
            case SDL_KEYDOWN:
            {
                if(Event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                {
                    Running = 0;
                }
                break;
            }
            case SDL_KEYUP:
            {
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

// TODO: Setup github and upload the first commit
// TODO: Setup Vulkan SDK. Include in project???
// TODO: Create new github repositores that hold libSDL, set directories inside libraries as git repos?

int main(int Argc, char **Argv)
{
    Argc; Argv;

    i32 Width = 1280;
    i32 Height = 720;
    SDL_Window *Window = SDL_CreateWindow("Catnip", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN);

    while(Running)
    {
        HandleEvents();
    }

    SDL_DestroyWindow(Window);

    return 0;
}
