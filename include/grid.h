#ifndef GRID_H
#define GRID_H

#include <SDL3/SDL.h>
#include <vector>
#include "tile.h"

class Grid
{
public:
    Grid(int col, int row, int tileSize, SDL_Texture* spriteSheet);

    ~Grid();

    void draw(SDL_Renderer* renderer, int windowWidth, int windowHeight) const;

    Tile const& getTile(int x, int y) const;

    Tile& getTile(int x, int y);

    std::vector<std::vector<Tile>> getTiles();

    void drawGrid(SDL_Renderer* renderer, int windowWidth, int windowHeight) const;

    void setHighlight(float x, float y, bool state);

    void setType(float x, float y, TileType type);

    void setSpriteType(const SDL_Point &position, SpriteType type);

    bool isWalkable(int x, int y) const;

    void showGrid();

    SDL_Texture* getSpriteSheet() const;

    int convertCoordinateToIndex(float coordinate) const;
    SDL_Point convertCoordinateToIndex(float x, float y) const;

    void cleanHighlights();

private:
    int columns;
    int rows;
    int tileSize;
    bool displayGrid;
    std::vector<std::vector<Tile>> tiles;
    SDL_Texture* spriteSheet;
};

#endif