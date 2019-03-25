/**
 * @file     RenderManager.hpp
 * @author   Marek Cernak
 * @date     3/25/2019
 */

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
        /**
         * Render manager class used to create new sdl textures.
         * TODO: add drawing and font drawing/creating responsibility here.
         */
        class ENGINE_API RenderManager
        {
        public:

            /**
             * Loads a texture from file and creates new SDL_Texture. Reuses the texture if already loaded before.
             * renderer [in] SDL renderer.
             * path [in] Path to texture file. 
             * @return Loaded texture.
             */
            static SDL_Texture* CreateTexture(SDL_Renderer* renderer, const char* path);

            /**
             * Destroys the loaded textures.
             */
            static void DestroyTextures();

        private:
            static std::unordered_map<std::string, SDL_Texture*> m_textureMap;
        };
    }
}