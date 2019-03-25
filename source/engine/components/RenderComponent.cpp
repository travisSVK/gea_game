/**
 * @file     RenderComponent.cpp
 * @author   Marek Cernak
 * @date     3/24/2019
 */

#include "RenderComponent.hpp"

namespace engine
{
    namespace components
    {
        void RenderComponent::Create(managers::EntityManager::Entity entity, const glm::dvec2& position, MovementType movement)
        {
            m_entity = entity;
            m_spritePosition = position;
            m_enabled = true;
            m_movement = movement;
        }

        managers::EntityManager::Entity RenderComponent::GetEntity()
        {
            return m_entity;
        }

        void RenderComponent::SetPosition(const glm::dvec2& position)
        {
            m_lastSpritePosition = m_spritePosition;
            m_spritePosition += position;
        }

        glm::dvec2 RenderComponent::GetSpritePosition()
        {
            return m_spritePosition;
        }

        MovementType RenderComponent::GetMovementType()
        {
            return m_movement;
        }

        void RenderComponent::Enable()
        {
            m_enabled = true;
        }

        void RenderComponent::Disable()
        {
            m_enabled = false;
        }

        bool RenderComponent::IsEnabled()
        {
            return m_enabled;
        }
    }
}