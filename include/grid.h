#ifndef GRID_H
#define GRID_H

#include <SDL3/SDL.h>
#include <vector>
#include <tile.h>


class Grid {
public:
    Grid(int col, int row, int tileSize, SDL_Texture *spriteSheet);

    ~Grid();

    void draw(SDL_Renderer *renderer, int windowWidth, int windowHeight) const;

    Tile const &getTile(float x, float y) const;

    Tile &getTile(float x, float y);

    void drawGrid(SDL_Renderer *renderer, int windowWidth, int windowHeight) const;

    void setHighlight(float x, float y, bool state);

    void setType(float x, float y, TileType type);

    SDL_Texture *getSpriteSheet() const;

private:
    int columns;
    int rows;
    int tileSize;
    std::vector<Tile> tiles;
    SDL_Texture *spriteSheet;
};

#endif