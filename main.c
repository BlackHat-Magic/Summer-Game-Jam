#define SDL_MAIN_USE_CALLBACKS 1 // use callbacks instead of main

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

static SDL_FPoint points[500];

// runs at startup
SDL_AppResult SDL_AppInit (void** appstate, int argc, char* argv[]) {
    SDL_SetAppMetadata ("Uh", "1.0", "xyz.lukeh.summer-game-jam-2025");

    if (!SDL_Init (SDL_INIT_VIDEO)) {
        SDL_Log ("Couldn't initialize SDL: %s", SDL_GetError ());
        return (SDL_APP_FAILURE);
    }
    if (!SDL_CreateWindowAndRenderer ("Uh", 640, 480, 0, &window, &renderer)) {
        SDL_Log ("Couldn't create window/renderer: %s", SDL_GetError ());
        return (SDL_APP_FAILURE);
    }

    // random points
    for (int i = 0; i < SDL_arraysize (points); i++) {
        points[i].x = (SDL_randf () * 440.0f) + 100.0f;
        points[i].y = (SDL_randf () * 280.0f) + 100.0f;
    }

    return SDL_APP_CONTINUE;
}

// runs when a new event (mosue input, keypress, etc.) happens
SDL_AppResult SDL_AppEvent (void* appstate, SDL_Event* event) {
    if (event->type == SDL_EVENT_QUIT) {
        return (SDL_APP_SUCCESS);
    }

    return (SDL_APP_CONTINUE);
}

// runs once each frame
SDL_AppResult SDL_AppIterate (void* appstate) {
    // time elapsed seconds
    const double now = ((double) SDL_GetTicks ()) / 1000.0;

    // ===== WINDOW COLOR =====
    const float red     = (float) (0.5 + 0.5 * SDL_sin (now));
    const float green   = (float) (0.5 + 0.5 * SDL_sin (now + SDL_PI_D * 2 / 3));
    const float blue    = (float) (0.5 + 0.5 * SDL_sin (now + SDL_PI_D * 4 / 3));
    SDL_SetRenderDrawColorFloat (renderer, red, green, blue, SDL_ALPHA_OPAQUE_FLOAT);
    
    SDL_RenderClear (renderer);
    // ===== END WINDOW COLOR =====

    // ===== RECTANGLE =====
    SDL_FRect rect;
    SDL_SetRenderDrawColor (renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
    rect.x = rect.y = 100;
    rect.w = 440;
    rect.h = 280;
    SDL_RenderFillRect (renderer, &rect);
    // ===== END RECTANGLE =====

    // ===== POINTS =====
    SDL_SetRenderDrawColor (renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderPoints (renderer, points, SDL_arraysize (points));
    // ===== END POINTS =====

    // ===== UNFILLED RECT =====
    SDL_SetRenderDrawColor (renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
    rect.x += 30;
    rect.y += 30;
    rect.w -= 60;
    rect.h -= 60;
    SDL_RenderRect (renderer, &rect);
    // ===== END UNFILLED RECT =====

    // ===== LINES =====
    SDL_SetRenderDrawColor (renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderLine (renderer, 0, 0, 650, 480);
    SDL_RenderLine (renderer, 0, 480, 640, 0);
    // ===== END LINES =====

    // put the newly-cleared renderer on the screen
    SDL_RenderPresent (renderer);

    return (SDL_APP_CONTINUE);
}

void SDL_AppQuit (void* appstate, SDL_AppResult result) {
    // ...
}