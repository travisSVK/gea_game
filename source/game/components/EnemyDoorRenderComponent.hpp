#pragma once
#include "components/RenderComponent.hpp"
#include <unordered_map>

struct SDL_Renderer;
struct SDL_Texture;
namespace game
{
    namespace components
    {
        class EnemyDoorRenderComponent : public engine::components::RenderComponent
        {
        public:

            virtual void Create(engine::managers::EntityManager::Entity entity, SDL_Renderer* renderer, const std::string& path, const glm::dvec2& position, engine::managers::EntityManager* entityManager);
            virtual engine::common::Event* Update() override;
            virtual void Destroy() override;
            virtual void Receive(engine::common::Event* message) override;
            virtual ComponentType GetComponentType() override;

        protected:
            virtual void Draw();
            virtual engine::common::Event* UpdateSpriteIndex();

        private:
            std::unordered_map<unsigned int, std::vector<std::pair<SDL_Texture*, glm::ivec2>>> m_spritesheet;
            SDL_Renderer* m_renderer;
            unsigned int m_spriteIndex;
            float m_indexChangeRate;
            float m_indexChangeRateTreshold;
            DoorState m_doorState;
            engine::managers::EntityManager* m_entityManager;
            std::string m_path;
        };
    }
}