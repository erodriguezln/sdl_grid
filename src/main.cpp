#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_mouse.h>
#include <grid.h>
#include <iostream>
#include <textureManager.h>
#include "bfs.cpp"

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define CELL_SIZE 40

typedef struct AppState
{
    Grid* grid;
    std::vector<std::vector<int>> binaryMatrix;
} AppState;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Sdl Grid", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer))
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // int col = WINDOW_WIDTH / CELL_SIZE;
    // int row = WINDOW_HEIGHT / CELL_SIZE;
    int col = 20;
    int row = 20;

    AppState* state = new AppState;

    SDL_Texture* spriteSheet = TextureManager::getInstance()->loadTexture(renderer, "assets/Textures-16.png");

    state->grid = new Grid(col, row, CELL_SIZE, spriteSheet);

    *appstate = state;

    std::vector<std::vector<int>> binaryMatrix(row, std::vector<int>(col, 0));
    state->binaryMatrix = binaryMatrix;


    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    AppState* state = (AppState*)appstate;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    float mouseX, mouseY;
    Uint32 mouseButton = SDL_GetMouseState(&mouseX, &mouseY);
    const bool* keys = SDL_GetKeyboardState(NULL);

    if (mouseButton == SDL_BUTTON_LEFT)
    {
        state->grid->setType(mouseX, mouseY, TileType::WALL);
    }
    if (mouseButton == SDL_BUTTON_X1)
    {
        state->grid->setType(mouseX, mouseY, TileType::FLOOR);
    }
    // TODO por que no funciona luego del primer intento
    if (keys[SDL_SCANCODE_X])
    {
        std::vector<std::vector<Tile>> test = state->grid->getTiles();
        for (int i = 0; i < test.size(); i++)
        {
            for (int j = 0; j < test[i].size(); j++)
            {
                if (test[i][j].isWalkable())
                {
                    state->binaryMatrix[i][j] = 1;
                }
            }
        }
        std::vector<SDL_Point> path = bfs(state->binaryMatrix);

        for (int i = 0; i < path.size(); i++)
        {
            state->grid->setHighlight(path[i].x, path[i].y, true);
        }

    }


    state->grid->draw(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    AppState* state = (AppState*)appstate;
    delete state->grid;
    delete state;
    delete (AppState*)appstate;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}