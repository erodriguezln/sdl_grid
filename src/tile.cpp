#include <tile.h>

Tile::Tile(TileType type): type(type), highlight(false) {
}

Tile::~Tile() {
}

TileType Tile::getType() const {
    return this->type;
}

void Tile::setType(TileType type) {
    this->type = type;
}

bool Tile::getHighlight() const {
    return this->highlight;
}

void Tile::setHighlight(const bool show) {
    this->highlight = show;
}

void Tile::setTexture(SDL_Texture *texture) {
    // this->spriteSheet = texture;
}

std::map<TileType, TileProperties> tileMap = {
    {TileType::GRASS, {{384, 256, 128, 128}, true}},
    {TileType::SAND, {{256, 0, 128, 128}, true}}
};

void Tile::render(SDL_Renderer *renderer, SDL_Texture *spriteSheet, TileType type, int x, int y, int size) const {
    SDL_FRect dstRect = {
        static_cast<float>(x), static_cast<float>(y), static_cast<float>(size), static_cast<float>(size)
    };
    SDL_FRect clip = tileMap[type].spriteClip;

    SDL_SetTextureColorMod(spriteSheet, 255, 255, 255);
    SDL_RenderTexture(renderer, spriteSheet, &clip, &dstRect);
}