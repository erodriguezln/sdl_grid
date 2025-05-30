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
    WALL,
  };

enum class SpriteType
{
    TICKET,
    X,
    NONE
  };

class Tile
{
public:
    Tile(TileType type);

    ~Tile();

    TileType getType() const;

    SpriteType getSpriteType() const;

    void setType(TileType type);

    void setSpriteType(SpriteType type);

    bool getHighlight() const;

    void setHighlight(bool show);

    void setTexture(SDL_Texture* texture);

    bool isWalkable() const;

    void render(SDL_Renderer* renderer, SDL_Texture* spriteSheet, TileType type, int x, int y, int size) const;
    void renderSprite(SDL_Renderer* renderer, SDL_Texture* spriteSheet, SpriteType type, int x, int y, int size) const;

private:
    TileType type;
    SpriteType spriteType;
    bool highlight;
};

#endif //TILE_H