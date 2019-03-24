#pragma once
#include "../EngineAPI.hpp"
#include <unordered_map>

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Surface;

namespace engine
{
    namespace managers
    {
        class ENGINE_API RenderManager
        {
        public:
            static SDL_Texture* CreateTexture(SDL_Renderer* renderer, const char* path);
            static SDL_Surface* CreateColorSurface(int width, int height);
            static void DestroyTextures();

        private:
            static std::unordered_map<std::string, SDL_Texture*> m_textureMap;
        };
    }
}