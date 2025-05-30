#define SDL_MAIN_USE_CALLBACKS 1

#include "bfs.h"
#include "grid.h"
#include "textureManager.h"
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_mouse.h>

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 800;
constexpr int CELL_SIZE = 16;

typedef struct AppState
{
	Grid* grid;
	std::vector<std::vector<int>> binaryMatrix;
	bool trackMouse = false;
	TileType currentTileType = TileType::FLOOR;
	SpriteType currentSpriteType = SpriteType::NONE;
	SDL_Point pathStart = { 0, 0 };
	SDL_Point pathEnd = { 0, 0 };
	int pathSteps = 0;
	float mouseX, mouseY;

} AppState;

void updateBinaryMatrix(AppState* state);
static void highlightPath(AppState* state, const std::vector<SDL_Point>& path);
void do_work(AppState* state);

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

	int scaledCellWidth = WINDOW_WIDTH / col;
	int scaledCellHeight = WINDOW_HEIGHT / row;
	int scaledCellSize = std::min(scaledCellWidth, scaledCellHeight);

	AppState* state = new AppState;

	SDL_Texture* spriteSheet = TextureManager::getInstance()->loadTexture(renderer, "assets/textures.png");

	state->grid = new Grid(col, row, scaledCellSize, spriteSheet);

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
		std::vector<SDL_Point> path;
		SDL_Point currentIndexPosition = state->grid->convertCoordinateToIndex(state->mouseX, state->mouseY);


		switch (event->key.scancode)
		{
		case SDL_SCANCODE_ESCAPE:
			return SDL_APP_SUCCESS;
		case SDL_SCANCODE_T:
			state->trackMouse = !state->trackMouse;
			state->grid->cleanHighlights();
			break;
		case SDL_SCANCODE_G:
			state->grid->showGrid();
			break;
		case SDL_SCANCODE_X:
			std::cout << state->pathStart.x << "," << state->pathStart.y << std::endl;
			std::cout << state->pathEnd.x << "," << state->pathEnd.y << std::endl;

			/*updateBinaryMatrix(state);
			path = bfs(state->binaryMatrix, state->pathStart, state->pathEnd);
			highlightPath(state, path);*/
			do_work(state);
			break;
		case SDL_SCANCODE_1:
			state->currentTileType = TileType::WALL;
			break;
		case SDL_SCANCODE_2:
			state->currentTileType = TileType::FLOOR;
			break;

		case SDL_SCANCODE_S:
			state->grid->setSpriteType(state->pathStart, SpriteType::NONE);
			state->grid->setSpriteType(currentIndexPosition, SpriteType::TICKET);
			state->pathStart = state->grid->convertCoordinateToIndex(state->mouseX, state->mouseY);
			break;
		case SDL_SCANCODE_E:
			state->grid->setSpriteType(state->pathEnd, SpriteType::NONE);
			state->grid->setSpriteType(currentIndexPosition, SpriteType::X);
			state->pathEnd = state->grid->convertCoordinateToIndex(state->mouseX, state->mouseY);
			break;
		}
	}

	return SDL_APP_CONTINUE;
}

static void highlightPath(AppState* state, const std::vector<SDL_Point>& path) {
	state->grid->cleanHighlights();

	for (const SDL_Point& point : path) {
		state->grid->setHighlight(point.x, point.y, true);
		//state->grid->setHighlight(point.x, point.y, true, HighlightColor::BLUE);
	}

}

void updateBinaryMatrix(AppState* state) {
	const std::vector<std::vector<Tile>> test = state->grid->getTiles();
	for (int i = 0; i < test.size(); i++)
	{
		for (int j = 0; j < test[i].size(); j++)
		{
			bool walkable = test[i][j].isWalkable();
			state->binaryMatrix[i][j] = walkable ? 1 : 0;
		}
	}
}

void do_work(AppState* state)
{
	updateBinaryMatrix(state);

	std::vector<SDL_Point> path = bfs(
		state->binaryMatrix,
		state->pathStart,
		state->pathEnd
	);

	highlightPath(state, path);
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
	AppState* state = (AppState*)appstate;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	//float mouseX, mouseY;
	Uint32 mouseButton = SDL_GetMouseState(&state->mouseX, &state->mouseY);
	const bool* keys = SDL_GetKeyboardState(NULL);

	if (state->mouseX < WINDOW_WIDTH && state->mouseX > 0 && state->mouseY < WINDOW_HEIGHT && state->mouseY > 0)
	{
		SDL_Point currentIndexPosition = state->grid->convertCoordinateToIndex(state->mouseX, state->mouseY);


		switch (mouseButton) {
		case SDL_BUTTON_LEFT:
			//state->grid->setSpriteType(mouseX, mouseY, state->currentSpriteType);
			state->grid->setType(state->mouseX, state->mouseY, state->currentTileType);
			break;
		case SDL_BUTTON_X1:
			state->grid->setType(state->mouseX, state->mouseY, state->currentTileType);
			state->grid->setSpriteType(currentIndexPosition, state->currentSpriteType);
			break;
		}
	}


	if (state->trackMouse)
	{
		state->pathEnd = state->grid->convertCoordinateToIndex(state->mouseX, state->mouseY);
		do_work(state);
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