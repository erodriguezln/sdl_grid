#ifndef TILE_H
#define TILE_H

#include "textureManager.h"

typedef struct TileProperties
{
    SDL_FRect spriteClip;
    bool walkable;
} TileProperties;

enum class TileType
{
    FLOOR,
    WALL
};

class Tile
{
public:
    Tile(TileType type);

    ~Tile();

    TileType getType() const;

    void setType(TileType type);

    bool getHighlight() const;

    void setHighlight(bool show);

    void setTexture(SDL_Texture* texture);

    bool isWalkable() const;

    void render(SDL_Renderer* renderer, SDL_Texture* spriteSheet, TileType type, int x, int y, int size) const;

private:
    TileType type;
    bool highlight;
};

#endif //TILE_H