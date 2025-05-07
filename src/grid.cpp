#include <grid.h>
#include <cstdio>
#include <textureManager.h>

Grid::Grid(int col, int row, int tileSize, SDL_Texture *spriteSheet) : columns(col), rows(row), tileSize(tileSize),
                                                                       spriteSheet(spriteSheet) {
    // Initializes the vector tiles with the specified size (row*col) and fills it with grass tiles
    tiles.resize(row * col, Tile{TileType::GRASS});
}

Grid::~Grid() {
}


Tile const &Grid::getTile(float x, float y) const {
    // Convert screen coordinates to grid indices
    // C++ casting from float to int truncates the decimal portion (floors for positive values)
    // This efficiently maps all coordinates within a cell to the same grid index
    int colIndex = static_cast<int>(x / this->tileSize);
    int rowIndex = static_cast<int>(y / this->tileSize);

    // if (rowIndex >= 0 && rowIndex < this->rows && colIndex >= 0 && colIndex < this->columns) {
    return this->tiles[rowIndex * this->columns + colIndex];
    // }
}

Tile &Grid::getTile(float x, float y) {
    int colIndex = static_cast<int>(x / this->tileSize);
    int rowIndex = static_cast<int>(y / this->tileSize);

    return this->tiles[rowIndex * this->columns + colIndex];
}

void Grid::drawGrid(SDL_Renderer *renderer, int windowWidth, int windowHeight) const {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    // Draw rows
    for (int i = 1; i < this->rows; i++) {
        int yPos = this->tileSize * i;
        SDL_RenderLine(renderer, 0, yPos, windowWidth, yPos);
    }

    // Draw columns
    for (int i = 1; i < this->columns; i++) {
        int xPos = this->tileSize * i;
        SDL_RenderLine(renderer, xPos, 0, xPos, windowHeight);
    }
}


void Grid::draw(SDL_Renderer *renderer, int windowWidth, int windowHeight) const {
    for (int index = 0; index < this->tiles.size(); index++) {
        int rowIndex = index / this->columns;
        int colIndex = index % this->columns;

        // TODO estandariza esto, los parametros son muy desordenados...
        Tile const &tile = getTile(colIndex * tileSize, rowIndex * tileSize);


        tile.render(renderer, spriteSheet, tiles[index].getType(), colIndex * tileSize, rowIndex * tileSize,
                    tileSize);
    }

    drawGrid(renderer, windowWidth, windowHeight);
}


void Grid::setHighlight(float x, float y, bool state) {
    Tile &tile = getTile(x, y);
    tile.setHighlight(state);
}

void Grid::setType(float x, float y, TileType type) {
    Tile &tile = getTile(x, y);
    tile.setType(type);
}

SDL_Texture *Grid::getSpriteSheet() const {
    return spriteSheet;
}