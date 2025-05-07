#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_mouse.h>
#include <grid.h>
#include <textureManager.h>

static SDL_Window *window = nullptr;
static SDL_Renderer *renderer = nullptr;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define CELL_SIZE 100

typedef struct AppState {
    Grid *grid;
} AppState;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Sdl Grid", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    int col = WINDOW_WIDTH / CELL_SIZE;
    int row = WINDOW_HEIGHT / CELL_SIZE;

    AppState *state = new AppState;

    SDL_Texture *spriteSheet = TextureManager::getInstance()->loadTexture(renderer, "assets/allSprites_retina.png");

    state->grid = new Grid(col, row, CELL_SIZE, spriteSheet);

    *appstate = state;

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    AppState *state = (AppState *) appstate;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    float mouseX, mouseY;
    Uint32 mouseButton = SDL_GetMouseState(&mouseX, &mouseY);

    if (mouseButton == SDL_BUTTON_LEFT) {
        // state->grid->setHighlight(mouseX, mouseY, true);
        state->grid->setType(mouseX, mouseY, TileType::SAND);
    }
    if (mouseButton == SDL_BUTTON_X1) {
        state->grid->setHighlight(mouseX, mouseY, false);
    }


    state->grid->draw(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    AppState *state = (AppState *) appstate;
    delete state->grid;
    delete state;
    delete (AppState *) appstate;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}