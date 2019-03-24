#pragma once
#include "components/RenderComponent.hpp"
#include <unordered_map>

struct SDL_Renderer;
struct SDL_Texture;

namespace game
{
    namespace components
    {
        class StaticRenderComponent : public engine::components::RenderComponent
        {
        public:
            StaticRenderComponent() = default;
            virtual void Create(engine::managers::EntityManager::Entity entity, SDL_Renderer* renderer, const std::string& path, const glm::dvec2& position, const glm::ivec2& size);
            virtual engine::common::Event* Update() override;
            virtual void Destroy() override;
            virtual void Receive(engine::common::Event* message) override;
            virtual ComponentType GetComponentType() override;

        protected:
            virtual void Draw();

        private:
            SDL_Renderer* m_renderer;
            SDL_Texture* m_texture;
            glm::ivec2 m_size;
        };
    }
}