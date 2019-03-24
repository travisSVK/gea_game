#pragma once
#include "components/RenderComponent.hpp"
#include <unordered_map>

struct SDL_Renderer;
struct SDL_Texture;
namespace game
{
    namespace components
    {
        class SpriteRenderComponent : public engine::components::RenderComponent
        {
        public:
            virtual void Create(engine::managers::EntityManager::Entity entity, SDL_Renderer* renderer, const std::string& path, const glm::dvec2& position, MovementType movementType);
            virtual engine::common::Event* Update() override;
            virtual void Destroy() override;
            virtual void Receive(engine::common::Event* message) override;
            virtual ComponentType GetComponentType() override;

        protected:
            virtual void Draw();

        private:
            SDL_Renderer* m_renderer;
            std::unordered_map<unsigned int, std::pair<SDL_Texture*, glm::ivec2>> m_sprites;
            float m_destroyedChangeRate;
            float m_destroyedChangeRateTreshold;
            std::string m_path;
        };
    }
}