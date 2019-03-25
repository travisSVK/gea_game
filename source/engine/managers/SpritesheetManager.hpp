/**
 * @file     SpritesheetManager.hpp
 * @author   Marek Cernak
 * @date     3/25/2019
 */

#pragma once
#include "../EngineAPI.hpp"
#include <unordered_map>
#include "GLM/glm.hpp"

struct SDL_Texture;
struct SDL_Renderer;

namespace engine
{
    namespace managers
    {
        /**
         * Spritesheet manager class used to load spritesheets and sprites from file.
         * TODO: describe the expected file format here.
         */
        class ENGINE_API SpritesheetManager
        {
        public:

            /**
             * Parses the spritesheet definition file.
             * jsonPath [in] Path to spritesheet definition file.
             * renderer [in] SDL_Renderer.
             * @return unordered_map, where key = enum type (e.g. movement type), value = vector of pairs of textures and their respective sizes.
             */
            static std::unordered_map<unsigned int, std::vector<std::pair<SDL_Texture*, glm::ivec2>>> ParseSpritesheet(const char* jsonPath, SDL_Renderer* renderer);
            
            /**
             * Parses the sprites definition file.
             * jsonPath [in] Path to sprites definition file.
             * renderer [in] SDL_Renderer.
             * @return unordered_map, where key = enum type (e.g. movement type), value = pair of texture and its respective size.
             */
            static std::unordered_map<unsigned int, std::pair<SDL_Texture*, glm::ivec2>> ParseSprites(const char* jsonPath, SDL_Renderer* renderer);
        };
    }
}