#include "StaticRenderComponent.hpp"
#include "common/Event.hpp"
#include "managers/RenderManager.hpp"

namespace game
{
    namespace components
    {
        void StaticRenderComponent::Create(engine::managers::EntityManager::Entity entity, SDL_Renderer* renderer, const std::string& path, const glm::dvec2& position, const glm::ivec2& size)
        {
            RenderComponent::Create(entity, position, MovementType::NO_MOVEMENT);
            m_renderer = renderer;
            m_size = size;
            m_texture = engine::managers::RenderManager::CreateTexture(renderer, path.c_str());
        }

        engine::common::Event* StaticRenderComponent::Update()
        {
            Draw();
            return nullptr;
        }

        void StaticRenderComponent::Draw()
        {
            SDL_Rect rect = { m_spritePosition.x, m_spritePosition.y, m_size.x, m_size.y };

            //Render texture to screen
            SDL_RenderCopy(m_renderer, m_texture, NULL, &rect);
        }

        void StaticRenderComponent::Receive(engine::common::Event* message)
        {
        }

        ComponentType StaticRenderComponent::GetComponentType()
        {
            return StaticRender;
        }

        void StaticRenderComponent::Destroy()
        {
        }
    }
}