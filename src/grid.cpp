#include <grid.h>
#include <cstdio>

Grid::Grid(int col, int row, int cellSize) {
    this->col = col;
    this->row = row;
    this->cellSize = cellSize;
    this->cells = new Cell *[this->row];

    for (int rowIndex = 0; rowIndex < this->row; rowIndex++) {
        this->cells[rowIndex] = new Cell[this->col];
        for (int colIndex = 0; colIndex < this->col; colIndex++) {
            this->cells[rowIndex][colIndex].x = colIndex * this->cellSize;
            this->cells[rowIndex][colIndex].y = rowIndex * this->cellSize;

//            this->cells[rowIndex][colIndex].x2 = colIndex * this->cellSize;
//            this->cells[rowIndex][colIndex].y2 = rowIndex * this->cellSize;

            this->cells[rowIndex][colIndex].state = false;
        }
    }
}

Grid::~Grid() {

    for (int i = 0; i < this->row; i++) {
        delete[] this->cells[i];
    }
    delete[] this->cells;
}

void Grid::draw(SDL_Renderer *renderer, int windowWidth, int windowHeight) const {
    for (int rowIndex = 0; rowIndex < this->row; rowIndex++) {
        for (int colIndex = 0; colIndex < this->col; colIndex++) {
            if (this->cells[rowIndex][colIndex].state) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            }
            SDL_FRect rect;
            rect.x = colIndex * this->cellSize;
            rect.y = rowIndex * this->cellSize;
            rect.h = this->cellSize;
            rect.w = this->cellSize;
            SDL_RenderFillRect(renderer, &rect);
        }
    }


    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    // Draw rows
    for (int i = 1; i < this->row; i++) {
        int yPos = this->cellSize * i;
        SDL_RenderLine(renderer, 0, yPos, windowWidth, yPos);
    }

    // Draw columns
    for (int i = 1; i < this->col; i++) {
        int xPos = this->cellSize * i;
        SDL_RenderLine(renderer, xPos, 0, xPos, windowHeight);
    }


}

void Grid::setState(float x, float y, bool state) {
    // Convert screen coordinates to grid indices
    // C++ casting from float to int truncates the decimal portion (floors for positive values)
    // This efficiently maps all coordinates within a cell to the same grid index
    int rowIndex = static_cast<int>(y / this->cellSize);
    int colIndex = static_cast<int>(x / this->cellSize);

    printf("x: %.2f, y: %.2f, rowIndex: %d, colIndex %d\n", x, y, rowIndex, colIndex);

    if (rowIndex >= 0 && rowIndex < this->row && colIndex >= 0 && colIndex < this->col) {
        this->cells[rowIndex][colIndex].state = state;
    }

}