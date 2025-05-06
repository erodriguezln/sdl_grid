#include <SDL3/SDL.h>
#include <vector>

typedef struct Cell {
    bool state = false;
} Cell;

class Grid {
public:
    Grid(int col, int row, int cellSize);

    ~Grid();

    void draw(SDL_Renderer *renderer, int windowWidth, int windowHeight) const;

    inline Cell &getCell(int colIndex, int rowIndex);

    inline const Cell &getCell(int colIndex, int rowIndex) const;


    void setState(float x, float y, bool state);

private:
    int columns;
    int rows;
    int cellSize;
    std::vector<Cell> cells;


};