#pragma once
#include "../EngineAPI.hpp"

struct SDL_Texture;
struct SDL_Renderer;
class ENGINE_API RenderManager
{
public:
    static SDL_Texture* CreateTexture(SDL_Renderer* renderer, const char* path);
};