#ifdef _WIN32
#include <windows.h>
#endif
#include <SDL2/SDL.h>
#include <stdio.h>
extern const int SPRITE_SIZE = 32;
extern const int SCALE = 4;
const int TILE_SIZE = 20;

struct Player {
    int tile_x;
    int tile_y;
    int direction;
    float speed;
    int target_x;
    int target_y;
    int moving; // 0 or 1
    float move;
};

void render_gamestate(Uint32 ticks, SDL_Rect *sprite_atlas_locations, SDL_Rect *sprite_dest_locations, SDL_Texture **textures, SDL_Renderer* renderer, struct Player player) {
    
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

    // Render player
    int player_x = (player.tile_x * TILE_SIZE * SCALE *(1-player.move)) + (player.target_x* TILE_SIZE * SCALE * player.move); // interpolate between tile and target
    int player_y = (player.tile_y * TILE_SIZE * SCALE *(1-player.move)) + (player.target_y* TILE_SIZE * SCALE * player.move); // interpolate between tile and target
    SDL_Rect player_rect = {player_x, player_y, SPRITE_SIZE * SCALE, SPRITE_SIZE * SCALE};
    int orientation;
    switch (player.direction) {
        case 0:
            orientation = 0;
            break;
        case 1:
            orientation = 0;
            break;
        case 2: 
            orientation = 1;
            break;
        case 3:
            orientation = 1;
            break;
    }
    SDL_RenderCopyEx(renderer, textures[0], &sprite_atlas_locations[0], &player_rect, 0, NULL, orientation);

}

struct Player init_gamestate(SDL_Rect **sprite_atlas_locations, SDL_Rect **sprite_dest_locations, int num_sprites) {
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

    // initialize the player
    struct Player player = {0, 0, 0, 250.0f, 0, 0, FALSE, 0.0};
    return player;

    // initialize the map
    char map[48][24] = {};
}

void player_update(struct Player *playerpointer, float delta_time, char **map) {

    // make a copy of the player for convenience
    struct Player player = *playerpointer;

    // update player position
    if (player.tile_x != player.target_x || player.tile_y != player.target_y) {

        // check for collision with walls and edges of the map
        if (player.target_x == -1 || player.target_x == 47 || player.target_y == -1 || player.target_y == 23) { // if the player is at the edge of the map
            player.target_x = player.tile_x;
            player.target_y = player.tile_y;
        }
        if (map[player.target_x][player.target_y] == 1) { // if target is a wall
            player.target_x = player.tile_x;
            player.target_y = player.tile_y;
        }

        player.move += delta_time / player.speed; // percent of the way to the target
        if (player.move > 1.0f) {
            player.tile_x = player.target_x;
            player.tile_y = player.target_y;
            player.move = 0.0f; // reset move  
            if (player.moving) {
                switch (player.direction) {
                    case 0:
                        player.target_y = player.tile_y - 1;
                        break;
                    case 1:
                        player.target_x = player.tile_x + 1;
                        break;
                    case 2:
                        player.target_y = player.tile_y + 1;
                        break;
                    case 3:
                        player.target_x = player.tile_x - 1;
                        break;
                }
            }
        }
    }

    // update the player pointer
    *playerpointer = player;
}