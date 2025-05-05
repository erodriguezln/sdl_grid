#include <SDL3/SDL.h>

typedef struct Cell {
    int x;
    int y;
    int x2;
    int y2;
    bool state;
} Cell;

class Grid {
public:
    Grid(int col, int row, int cellSize);

    ~Grid();

    void draw(SDL_Renderer *renderer, int windowWidth, int windowHeight) const;

    void setState(float x, float y, bool state);

private:
    int col;
    int row;
    int cellSize;
    Cell **cells;
};