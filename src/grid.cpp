#include <grid.h>
#include <cstdio>
#include <iostream>
#include <textureManager.h>

Grid::Grid(int col, int row, int tileSize, SDL_Texture* spriteSheet) :
    columns(col),
    rows(row),
    tileSize(tileSize),
    spriteSheet(spriteSheet),
    tiles(row, std::vector<Tile>(col, Tile(TileType::FLOOR)))
{
}

Grid::~Grid()
{
}

Tile const& Grid::getTile(int x, int y) const
{
    return this->tiles[x][y];
}

Tile& Grid::getTile(int x, int y)
{
    return this->tiles[x][y];
}

std::vector<std::vector<Tile>>& Grid::getTiles()
{
    return tiles;
}

void Grid::drawGrid(SDL_Renderer* renderer, int windowWidth, int windowHeight) const
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    // Draw rows
    for (int i = 1; i < this->rows; i++)
    {
        int yPos = this->tileSize * i;
        SDL_RenderLine(renderer, 0, yPos, windowWidth, yPos);
    }

    // Draw columns
    for (int i = 1; i < this->columns; i++)
    {
        int xPos = this->tileSize * i;
        SDL_RenderLine(renderer, xPos, 0, xPos, windowHeight);
    }
}

void Grid::draw(SDL_Renderer* renderer, int windowWidth, int windowHeight) const
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            Tile const& tile = getTile(i, j);
            tile.render(renderer, spriteSheet, tiles[i][j].getType(), i * tileSize, j * tileSize, tileSize);
            isWalkable(i, j);
        }
    }

    drawGrid(renderer, windowWidth, windowHeight);
}

void Grid::setHighlight(float x, float y, bool state)
{
    // TODO this is not scalable, better to make a struct points or something
    // Tile& tile = getTile(convertCoordinateToIndex(x), convertCoordinateToIndex(y));
    Tile& tile = getTile(x, y);
    tile.setHighlight(state);
}

void Grid::setType(float x, float y, TileType type)
{
    // TODO this is not scalable, better to make a struct points or something
    Tile& tile = getTile(convertCoordinateToIndex(x), convertCoordinateToIndex(y));
    tile.setType(type);
}

bool Grid::isWalkable(int x, int y) const
{
    Tile tile = getTile(convertCoordinateToIndex(x), convertCoordinateToIndex(y));
    // std::cout << x << ", " << y << tile.isWalkable() << std::endl;
    return tile.isWalkable();
}

SDL_Texture* Grid::getSpriteSheet() const
{
    return spriteSheet;
}

int Grid::convertCoordinateToIndex(float coordinate) const
{
    return static_cast<int>(coordinate) / tileSize;
}