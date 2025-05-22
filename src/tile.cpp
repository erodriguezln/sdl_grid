#include <iostream>
#include <tile.h>

Tile::Tile(TileType type): type(type), highlight(false)
{
}

Tile::~Tile()
{
}

TileType Tile::getType() const
{
    return this->type;
}

void Tile::setType(TileType type)
{
    this->type = type;
}

bool Tile::getHighlight() const
{
    return this->highlight;
}

void Tile::setHighlight(const bool show)
{
    this->highlight = show;
}

void Tile::setTexture(SDL_Texture* texture)
{
    // this->spriteSheet = texture;
}


std::map<TileType, TileProperties> tileMap = {
    {TileType::FLOOR, {{96, 192, 16, 16}, true}},
    {TileType::WALL, {{0, 64, 16, 16}, false}}
};

// TODO temporary, really need a better approach
bool Tile::isWalkable() const
{
    // std::cout << tileMap[type].walkable << std::endl;
    return tileMap[type].walkable;
}

void Tile::render(SDL_Renderer* renderer, SDL_Texture* spriteSheet, TileType type, int x, int y, int size) const
{
    SDL_FRect dstRect = {
        static_cast<float>(x), static_cast<float>(y), static_cast<float>(size), static_cast<float>(size)
    };
    SDL_FRect clip = tileMap[type].spriteClip;


    if (this->getHighlight())
    {
        SDL_SetTextureColorMod(spriteSheet, 255, 0, 0);
    }
    else
    {
        SDL_SetTextureColorMod(spriteSheet, 255, 255, 255);
    }

    SDL_RenderTexture(renderer, spriteSheet, &clip, &dstRect);
}