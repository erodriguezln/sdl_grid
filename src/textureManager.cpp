#include "textureManager.h"


TextureManager::TextureManager() {
}

TextureManager::~TextureManager() {
    destroy();
}

// Singleton pattern, since i only need a single TextureManager
TextureManager* TextureManager::getInstance() {
    if (instance == nullptr) {
        instance = new TextureManager();
    }
    return instance;
}


SDL_Texture* TextureManager::loadTexture(SDL_Renderer* renderer, const std::string& path) {
    if (textureMap.find(path) != textureMap.end()) {
        return textureMap[path];
    }

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface == nullptr) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), SDL_GetError());
        return nullptr;
    }

    SDL_Texture* mTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    if (mTexture == nullptr) {
        printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        return nullptr;
    }

    textureMap.insert({ path, mTexture });

    return mTexture;
}

void TextureManager::destroy() {
    // range based map traversal
    for (const auto& pair : textureMap) {
        // value of the map element
        if (pair.second != nullptr) {
            SDL_DestroyTexture(pair.second);
        }
    }
    textureMap.clear();
}

TextureManager* TextureManager::instance = nullptr;