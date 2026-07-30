#include <midi/midi.h>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define FALSE 0
#define TRUE 1

typedef struct app_state
{
    unsigned int should_quit;
    SDL_Window *window;
    SDL_Renderer *render;

} app_t;

void init_sdl()
{
    int err = SDL_Init(SDL_INIT_EVERYTHING);
    if (err != 0)
    {
        printf("Failed to init SDL, reason: %s\n", SDL_GetError());
        exit(err);
    }
}

int main(void)
{
    app_t app = {0};

    // Initialise SDL first and get a window going.
    init_sdl();
    SDL_CreateWindowAndRenderer(
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN,
        &app.window,
        &app.render
    );
    app.should_quit = FALSE;
    while (!app.should_quit)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                app.should_quit = TRUE;
                break;
            
            default:
                printf("Unhandled event type: %d\n", e.type);
                break;
            }
        }

        SDL_SetRenderDrawColor(app.render, 0x94, 0x00, 0x84, 0xFF);
        SDL_RenderClear(app.render);
        
        SDL_SetRenderDrawColor(app.render, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderPresent(app.render);
    }

    return 0;
}