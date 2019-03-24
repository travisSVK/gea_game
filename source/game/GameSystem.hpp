#pragma once
#include "systems/System.hpp"
#include "managers/EntityManager.hpp"
#include "LevelNumbers.hpp"
#include "SDL_ttf.h"
#include <unordered_map>
#include "SDL_mixer.h"
#include "GLM/glm.hpp"

namespace engine
{
    namespace systems
    {
        class RenderSystem;
        class CollisionSystem;
        class MovementSystem;
        class InputSystem;
        class CameraSystem;
    }
}

namespace game
{
    class GameSystem : public engine::systems::System
    {
    public:

        GameSystem();
        bool Init(int width, int height);
        void Create();
        void Draw();
        void Update();
        void Destroy();
        float GetElapsedTime();
        bool GameOver();

    protected:
        void CreateLevel(engine::systems::RenderSystem* renderSystem,
            engine::systems::CollisionSystem* collisionSystem, 
            engine::systems::MovementSystem* movementSystem, 
            engine::systems::InputSystem* inputSystem, 
            engine::systems::CameraSystem* cameraSystem);
        virtual void DisableComponents(int excludeEntity);

    private:

        void Receive(engine::common::Event* event);
        void DrawText(int x, int y, const char * msg);
        void SwapBuffers();
        void ClearWindow();
        void DrawHUD();
        void RestartLevel();

    private:

        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        TTF_Font* m_font;
        engine::managers::EntityManager* m_entityManager;
        std::vector<System*> m_systems;
        std::unordered_map<SystemType, System*> m_systemsMap;
        LevelNumbers m_levelNumbers;
        engine::managers::EntityManager::Entity m_playerEntity;
        glm::dvec2 m_playerPosition;
        glm::dvec2 m_playerLastPosition;
        engine::managers::EntityManager::Entity m_carEntity;
        engine::managers::EntityManager::Entity m_gameEntity;
        Mix_Music* m_backgroundMusic;
        Mix_Chunk* m_shoot;
        Mix_Chunk* m_hit;
        Mix_Chunk* m_failed;
        Mix_Chunk* m_car;
        unsigned int m_playerScore;
        bool m_levelOver;
        bool m_levelFinished;
        bool m_drawScore;
        bool m_gameOver;
        bool m_gameStart;
        bool m_playerTemporaryDisabled;
        bool m_bonusScore;
        unsigned int m_playerLives;
        unsigned int m_buildingLevel;
        float m_playerHorizontalPosition;
        SDL_Texture* m_hudTexture;
        SDL_Texture* m_hudLives;
    };
}