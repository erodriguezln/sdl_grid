#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_mouse.h>
#include <grid.h>
#include <iostream>
#include <textureManager.h>
#include <bfs.h>

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

    const std::vector<std::vector<int>> binaryMatrix(row, std::vector<int>(col, 0));
    state->binaryMatrix = binaryMatrix;


    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    AppState* state = (AppState*)appstate;

    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }

    if (event->type == SDL_EVENT_KEY_DOWN && event->key.repeat == 0)
    {
        if (event->key.scancode == SDL_SCANCODE_G)
        {
            state->grid->showGrid();
        }
    }
    if (event->type == SDL_EVENT_KEY_DOWN && event->key.repeat == 0)
    {
        if (event->key.scancode == SDL_SCANCODE_X)
        {
            const std::vector<std::vector<Tile>> test = state->grid->getTiles();
            for (int i = 0; i < test.size(); i++)
            {
                for (int j = 0; j < test[i].size(); j++)
                {
                    bool walkable = test[i][j].isWalkable();
                    state->binaryMatrix[i][j] = walkable ? 1 : 0;
                    state->grid->setHighlight(i, j, false);
                }
            }
            std::vector<SDL_Point> path = bfs(state->binaryMatrix);

            for (int i = 0; i < path.size(); i++)
            {
                state->grid->setHighlight(path[i].x, path[i].y, true);
            }
        }
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
        if (mouseX < WINDOW_WIDTH && mouseX > 0 && mouseY < WINDOW_HEIGHT && mouseY > 0)
        {
            // std::cout << "Mouse X: " << mouseX << " Mouse Y: " << mouseY << std::endl;
            state->grid->setType(mouseX, mouseY, TileType::WALL);
        }
    }
    if (mouseButton == SDL_BUTTON_X1)
    {
        if (mouseX < WINDOW_WIDTH && mouseX > 0 && mouseY < WINDOW_HEIGHT && mouseY > 0)
        {
            state->grid->setType(mouseX, mouseY, TileType::FLOOR);
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