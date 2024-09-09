#ifdef _WIN32
#include <windows.h>
#endif
#include <SDL2/SDL.h>
#include <stdio.h>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const int SPRITE_SIZE = 32;
const int SCALE = 4;

#include "gamestate.c"

#ifdef _WIN32
// Define WinMain to call the regular main function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    // Parse command-line arguments using GetCommandLineA (ANSI version)
    LPSTR cmdLine = GetCommandLineA(); // Get the full command-line as a single string
    int argc = 0;
    char **argv = NULL;

    // Use a basic approach to split the command line into argc and argv
    // For simplicity, just handle it as a single argument case
    argc = 1;
    argv = (char **)malloc(sizeof(char *));
    argv[0] = cmdLine; // Treat the whole command line as a single argument
    
    // Open a console to show the debug output
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);

    // Call main with the parsed arguments
    int result = main(argc, argv);

    // Clean up (no dynamic memory allocation here, so no need for free())
    return result;
}
#endif

// Declare the function from the DLL
void perform_request(const char* submit_url, const char* api_key, const char* user_agent, const char* content_type, const char* prompt);

int main(int argc, char* argv[]) {

    // CALL
    // Provide arguments for the function
    const char* submit_url = "https://queue.fal.run/fal-ai/flux/dev";
    const char* api_key = "Key 5ff46e36-d30e-4609-ab5a-c4c6fb0d1297:79e9099d823568f450be54e8f4a12c9e";
    const char* user_agent = "fal-client/0.2.2 (cpp)";
    const char* content_type = "application/json";
    const char* prompt = "A 2000s photograph of a Roman bust next to a swimming pool in a Southern French villa";

    // Call the function from the DLL
    // perform_request(submit_url, api_key, user_agent, content_type, prompt);    
    // END

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP); // for fullscreen
    //SDL_Window* window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0); // for windowed
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Raise and focus the window
    SDL_RaiseWindow(window);

    // Get actual window size
    int actual_width, actual_height;
    SDL_GetWindowSize(window, &actual_width, &actual_height);
    printf("Actual window size: %dx%d\n", actual_width, actual_height);


    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Load BMP texture
    SDL_Surface* bmp = SDL_LoadBMP("assets/characters.bmp");
    SDL_Surface* backgroundbmp = SDL_LoadBMP("assets/background.bmp");
    SDL_Surface* titlebmp = SDL_LoadBMP("assets/title.bmp");
    if (!bmp || !backgroundbmp || !titlebmp) {
        printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_Surface* bmp_surfaces[3] = {bmp, backgroundbmp, titlebmp};

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, bmp);
    SDL_Texture* background = SDL_CreateTextureFromSurface(renderer, backgroundbmp);
    SDL_Texture* title = SDL_CreateTextureFromSurface(renderer, titlebmp);
    SDL_FreeSurface(bmp);
    SDL_FreeSurface(backgroundbmp);
    SDL_FreeSurface(titlebmp);
    if (!texture || !background || !title) {
        printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_Texture* textures[3] = {texture, background, title};
    
    // intitialise state machine
    int state = 0;

    int running = 1;
    Uint32 start_ticks = SDL_GetTicks64();
    printf("start_ticks: %d\n", start_ticks);
    Uint32 ticks = 0;
    SDL_Event e;
    SDL_Rect *sprite_atlas_locations;
    SDL_Rect *sprite_dest_locations;
    struct Player player = init_gamestate(&sprite_atlas_locations, &sprite_dest_locations, 3);

    SDL_Rect title_rect = {actual_width / 6, actual_height / 6, (actual_width*2)/3, (actual_height*2)/3};

    while (running) {
        //printf("player.move: %d\n", player.move);
        //printf("player.target_x: %d\n", player.target_x);
        //printf("player.target_y: %d\n", player.target_y);
        //printf("player.moving: %d\n", player.moving);
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
                running = 0;
            }
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN && state == 0) {
                state = 1;
            }
            else if (state == 1) {
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_UP && !player.moving) {
                    player.target_y = player.tile_y - 1;
                    player.direction = 0;
                    player.moving = TRUE;
                }
                else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_DOWN && !player.moving) {
                    player.target_y = player.tile_y + 1;
                    player.direction = 2;
                    player.moving = TRUE;
                }
                else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_LEFT && !player.moving) {
                    player.target_x = player.tile_x - 1;
                    player.direction = 3;
                    player.moving = TRUE;
                }
                else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RIGHT && !player.moving) {
                    player.target_x = player.tile_x + 1;
                    player.direction = 1;
                    player.moving = TRUE;
                }
                else if (e.type == SDL_KEYUP && (e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_UP)) {
                    player.moving = FALSE;
                }
                else if (e.type == SDL_KEYUP && (e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_LEFT)) {
                    player.moving = FALSE;
                }
            }
        }
        //printf("delta_time: %d\n", ticks);
        //printf("delta_time: %d\n", SDL_GetTicks64() - start_ticks);
        float delta_time = (float)(SDL_GetTicks64() - ticks - start_ticks);
        ticks = SDL_GetTicks64() - start_ticks;
        printf("delta_time: %f\n", delta_time);
        // Clear the screen
        SDL_RenderClear(renderer);

        // first state(main menu)
        if (state == 0) {
            // Render background
            SDL_RenderCopy(renderer, background, NULL, NULL);
            // Render menu
            SDL_RenderCopy(renderer, title, NULL, &title_rect);
        }
        // second state(game)
        else if (state == 1) {
            render_gamestate(ticks, sprite_atlas_locations, sprite_dest_locations, textures, renderer, player);
            player_update(&player, delta_time);
        }
        // Present the frame
        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }
    free(sprite_atlas_locations);
    free(sprite_dest_locations);
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(background);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}