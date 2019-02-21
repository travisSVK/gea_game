#include "RenderManager.hpp"
#include "SDL_image.h"

SDL_Texture* RenderManager::CreateTexture(SDL_Renderer* renderer, const char* path)
{
    SDL_Surface* surf = IMG_Load(path);
    if (!surf)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load image %s! SDL_image Error: %s\n", path, SDL_GetError());
        return nullptr;
    }

    //Create texture from surface pixels
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
    if (!texture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
        return nullptr;
    }
    SDL_FreeSurface(surf);
    return texture;
}