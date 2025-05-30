#include "tile.h"
#include <iostream>

Tile::Tile(TileType type) : type(type), highlight(false), spriteType(SpriteType::NONE)
{
}

Tile::~Tile()
{
}

TileType Tile::getType() const
{
  return this->type;
}

SpriteType Tile::getSpriteType() const
{
  return this->spriteType;
}

void Tile::setType(TileType type)
{
  this->type = type;
}

void Tile::setSpriteType(SpriteType type)
{
  this->spriteType = type;
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
    {TileType::FLOOR, {{32, 16, 16, 16}, true}},
    {TileType::WALL, {{16, 240, 16, 16}, false}},
};

std::map<SpriteType, TileProperties> spriteMap = {
    {SpriteType::TICKET, {{704, 768, 16, 16}, true}},
    {SpriteType::X, {{688, 768, 16, 16}, true}},
    {SpriteType::NONE, {{496, 0, 16, 16}, true}},
};

// TODO temporary, really need a better approach
bool Tile::isWalkable() const
{
  if (tileMap[type].walkable && spriteMap[spriteType].walkable)
  {
    return true;
  }
  return false;
}

void Tile::render(SDL_Renderer* renderer, SDL_Texture* spriteSheet, TileType type, int x, int y, int size) const
{
  SDL_FRect dstRect = {
      static_cast<float>(x * size), static_cast<float>(y * size), static_cast<float>(size), static_cast<float>(size)
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

void Tile::renderSprite(SDL_Renderer* renderer, SDL_Texture* spriteSheet, SpriteType type, int x, int y, int size) const
{
  SDL_FRect dstRect = {
      static_cast<float>(x * size), static_cast<float>(y * size), static_cast<float>(size), static_cast<float>(size)
  };
  SDL_FRect clip = spriteMap[type].spriteClip;


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