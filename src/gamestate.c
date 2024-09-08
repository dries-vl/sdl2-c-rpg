#ifdef _WIN32
#include <windows.h>
#endif
#include <SDL2/SDL.h>
#include <stdio.h>
extern const int SPRITE_SIZE = 32;
extern const int SCALE = 4;

void render_gamestate(Uint32 ticks, SDL_Rect *sprite_atlas_locations, SDL_Rect *sprite_dest_locations, SDL_Texture **textures, SDL_Renderer* renderer) {
    
    // Update animations
    sprite_atlas_locations[0].x = SPRITE_SIZE * ((ticks / 100) % 4);
    sprite_dest_locations[0].x = ((ticks / 14) % 768) - 128;

    sprite_atlas_locations[1].x = SPRITE_SIZE * ((ticks / 100) % 4);
    sprite_dest_locations[1].x = -(((ticks / 12) % 768) - 672);

    sprite_atlas_locations[2].x = SPRITE_SIZE * ((ticks / 100) % 4);
    sprite_dest_locations[2].x = ((ticks / 10) % 768) - 128;

    // Render background
    SDL_RenderCopy(renderer, textures[1], NULL, NULL);

    // Render sprites
    SDL_RenderCopy(renderer, textures[0], &sprite_atlas_locations[0], &sprite_dest_locations[0]);
    SDL_RenderCopyEx(renderer, textures[0], &sprite_atlas_locations[1], &sprite_dest_locations[1], 0, NULL, SDL_FLIP_HORIZONTAL);
    SDL_RenderCopy(renderer, textures[0], &sprite_atlas_locations[2], &sprite_dest_locations[2]);
}

void init_gamestate(SDL_Rect **sprite_atlas_locations, SDL_Rect **sprite_dest_locations, int num_sprites) {
    // Allocate memory for the arrays
    *sprite_atlas_locations = malloc(num_sprites * sizeof(SDL_Rect));
    *sprite_dest_locations = malloc(num_sprites * sizeof(SDL_Rect));

    if (*sprite_atlas_locations == NULL || *sprite_dest_locations == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    // Initialize the rectangles
    (*sprite_atlas_locations)[0] = (SDL_Rect){0, 0, SPRITE_SIZE, SPRITE_SIZE};
    (*sprite_atlas_locations)[1] = (SDL_Rect){0, 32, SPRITE_SIZE, SPRITE_SIZE};
    (*sprite_atlas_locations)[2] = (SDL_Rect){0, 64, SPRITE_SIZE, SPRITE_SIZE};

    (*sprite_dest_locations)[0] = (SDL_Rect){0, 0, SPRITE_SIZE * SCALE, SPRITE_SIZE * SCALE};
    (*sprite_dest_locations)[1] = (SDL_Rect){0, 32, SPRITE_SIZE * SCALE, SPRITE_SIZE * SCALE};
    (*sprite_dest_locations)[2] = (SDL_Rect){0, 64, SPRITE_SIZE * SCALE, SPRITE_SIZE * SCALE};
}