#pragma once
#include "../EngineAPI.hpp"
#include <unordered_map>
#include "GLM/glm.hpp"

struct SDL_Texture;
struct SDL_Renderer;
class ENGINE_API SpritesheetManager
{
public:
    static std::unordered_map<unsigned int, std::vector<std::pair<SDL_Texture*, glm::ivec2>>> ParseSpritesheet(const char* jsonPath, SDL_Renderer* renderer);
    static std::unordered_map<unsigned int, std::pair<SDL_Texture*, glm::ivec2>> ParseSprites(const char* jsonPath, SDL_Renderer* renderer);
};