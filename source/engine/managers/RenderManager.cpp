/**
 * @file     RenderManager.cpp
 * @author   Marek Cernak
 * @date     3/25/2019
 */

#include "RenderManager.hpp"
#include "SDL_image.h"

namespace engine
{
    namespace managers
    {
        std::unordered_map<std::string, SDL_Texture*> RenderManager::m_textureMap;

        SDL_Texture* RenderManager::CreateTexture(SDL_Renderer* renderer, const char* path)
        {
            std::string stringPath = std::string(path);
            if (m_textureMap.find(stringPath) != m_textureMap.end())
            {
                return m_textureMap[stringPath];
            }
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
            m_textureMap[stringPath] = texture;
            return texture;
        }

        void RenderManager::DestroyTextures()
        {
            for (const auto& texture : m_textureMap)
            {
                SDL_DestroyTexture(texture.second);
            }
        }
    }
}