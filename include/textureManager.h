#ifndef TEXTURE_H
#define TEXTURE_H

#include <map>
#include <SDL3_image/SDL_image.h>
#include <string>


class TextureManager {
public:
    ~TextureManager();

    static TextureManager* getInstance();

    // Pass path by reference for performance reasons (avoids copying)
    // Doesn't even matter, but it's a good practice to learn
    SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& path);

    void destroy();

private:
    // singleton pattern
    static TextureManager* instance;
    std::map<std::string, SDL_Texture*> textureMap;

    // private constructor, singleton pattern
    TextureManager();
};


#endif //TEXTURE_H