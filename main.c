#define SDL_MAIN_USE_CALLBACKS 1 // use callbacks instead of main

#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* texture = NULL;
static int texture_width = 0;
static int texture_height = 0;

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

static SDL_FPoint points[500];

// runs at startup
SDL_AppResult SDL_AppInit (void** appstate, int argc, char* argv[]) {

    // ===== CREATE WINDOW =====
    SDL_SetAppMetadata ("Uh", "1.0", "xyz.lukeh.summer-game-jam-2025");
    if (!SDL_Init (SDL_INIT_VIDEO)) {
        SDL_Log ("Couldn't initialize SDL: %s", SDL_GetError ());
        return (SDL_APP_FAILURE);
    }
    if (!SDL_CreateWindowAndRenderer ("Uh", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
        SDL_Log ("Couldn't create window/renderer: %s", SDL_GetError ());
        return (SDL_APP_FAILURE);
    }
    // ===== END CREATE WINDOW =====

    // ===== TEXTURE =====
    SDL_Surface* surface = NULL;
    char* file_path = NULL;
    SDL_asprintf (&file_path, "%samongus.bmp", SDL_GetBasePath ());
    surface = IMG_Load (file_path);
    if (!surface) {
        SDL_Log ("Couldn't load bitmap: %s", SDL_GetError ());
        return (SDL_APP_FAILURE);
    }

    SDL_free (file_path);

    texture_width = surface->w;
    texture_height = surface->h;

    texture = SDL_CreateTextureFromSurface (renderer, surface);
    if (!texture) {
        SDL_Log ("Couldn't create static texture: %s", SDL_GetError ());
        return (SDL_APP_FAILURE);
    }

    // done with this; the texture has a copy of the pixels now
    SDL_DestroySurface (surface);
    // ===== END TEXTURE =====

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

// runs once each frsdl3 load png instead of bmpame
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

    // ===== TEXTURE =====
    SDL_FRect dst_rect;
    const Uint64 now2 = SDL_GetTicks();
    const float direction = ((now2 % 2000) >= 1000) ? 1.0f : -1.0f;
    const float scale = ((float) (((int) (now2 % 1000)) - 500) / 500.0f) * direction;
    
    dst_rect.x = (100.0f * scale);
    dst_rect.y = 0.0f;
    dst_rect.w = (float) texture_width;
    dst_rect.h = (float) texture_height;
    SDL_RenderTexture (renderer, texture, NULL, &dst_rect);

    dst_rect.x = ((float) (WINDOW_WIDTH - texture_width)) / 2.0f;
    dst_rect.y = ((float) (WINDOW_HEIGHT - texture_height)) / 2.0f;
    dst_rect.w = (float) texture_width;
    dst_rect.h = (float) texture_height;
    SDL_RenderTexture (renderer, texture, NULL, &dst_rect);

    dst_rect.x = ((float) (WINDOW_WIDTH - texture_width)) / 2.0f;
    dst_rect.y = (float) (WINDOW_HEIGHT - texture_height);
    dst_rect.w = (float) texture_width;
    dst_rect.h = (float) texture_height;
    SDL_RenderTexture (renderer, texture, NULL, &dst_rect);
    // ===== END TEXTURE =====

    // put the newly-cleared renderer on the screen
    SDL_RenderPresent (renderer);

    return (SDL_APP_CONTINUE);
}

void SDL_AppQuit (void* appstate, SDL_AppResult result) {
    // ...
}