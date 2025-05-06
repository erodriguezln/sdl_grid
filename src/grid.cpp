#include <grid.h>
#include <cstdio>

Grid::Grid(int col, int row, int cellSize) : columns(col), rows(row), cellSize(cellSize), cells(row * col, Cell{}) {}

Grid::~Grid() {}

inline Cell &Grid::getCell(int colIndex, int rowIndex) {
    return this->cells[rowIndex * this->columns + colIndex];
}

inline const Cell &Grid::getCell(int colIndex, int rowIndex) const {
    return this->cells[rowIndex * this->columns + colIndex];
}

void Grid::draw(SDL_Renderer *renderer, int windowWidth, int windowHeight) const {
    for (int index = 0; index < this->cells.size(); index++) {
        int rowIndex = index / this->columns;
        int colIndex = index % this->columns;

        if (getCell(colIndex, rowIndex).state) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        } else {
            continue;
        }

        SDL_FRect rect;
        rect.x = colIndex * this->cellSize;
        rect.y = rowIndex * this->cellSize;
        rect.h = this->cellSize;
        rect.w = this->cellSize;
        SDL_RenderFillRect(renderer, &rect);
    }


    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    // Draw rows
    for (int i = 1; i < this->rows; i++) {
        int yPos = this->cellSize * i;
        SDL_RenderLine(renderer, 0, yPos, windowWidth, yPos);
    }

    // Draw columns
    for (int i = 1; i < this->columns; i++) {
        int xPos = this->cellSize * i;
        SDL_RenderLine(renderer, xPos, 0, xPos, windowHeight);
    }


}

void Grid::setState(float x, float y, bool state) {
    // Convert screen coordinates to grid indices
    // C++ casting from float to int truncates the decimal portion (floors for positive values)
    // This efficiently maps all coordinates within a cell to the same grid index
    int colIndex = static_cast<int>(x / this->cellSize);
    int rowIndex = static_cast<int>(y / this->cellSize);

    printf("x: %.2f, y: %.2f, rowIndex: %d, colIndex %d\n", x, y, rowIndex, colIndex);

    if (rowIndex >= 0 && rowIndex < this->rows && colIndex >= 0 && colIndex < this->columns) {
        getCell(colIndex, rowIndex).state = state;
    }

}