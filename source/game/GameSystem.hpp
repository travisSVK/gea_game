#pragma once
#include "systems/System.hpp"
#include "SDL_ttf.h"
#include <unordered_map>
#include "systems/System.hpp"

class GameSystem : public System
{
public:

    struct KeyStatus
    {
        bool fire; // space
        bool left; // left arrow
        bool right; // right arrow
        bool esc; // escape button
    };

    GameSystem();
    bool Init(int width, int height);
    void Create();
    void Draw();
    void Update(float delta);
    void Destroy();
    float GetElapsedTime();
    bool GameOver();

private:
    
    // receive event from another system
    void Receive(Event* event);
    void ProcessInput();
    void DrawText(int x, int y, const char * msg);
    void SwapBuffers();
    void ClearWindow();

private:

    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    TTF_Font* font;
    EntityManager* m_entityManager;
    std::vector<System*> m_systems;

    EntityManager::Entity m_playerEntity;
    KeyStatus key;
    bool m_playerMoving;
    bool m_gameOver;
};