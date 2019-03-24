#include "MovementComponent.hpp"
#include <vector>

namespace engine
{
    namespace components
    {
        void MovementComponent::Create(managers::EntityManager::Entity entity, const glm::dvec2& position, DirectionType direction)
        {
            m_entity = entity;
            m_position = position;
            m_movement = NO_MOVEMENT;
            m_direction = direction;
            m_enabled = true;
        }

        managers::EntityManager::Entity MovementComponent::GetEntity()
        {
            return m_entity;
        }

        glm::dvec2 MovementComponent::GetPosition()
        {
            return m_position;
        }

        DirectionType MovementComponent::GetDirection()
        {
            return m_direction;
        }

        void MovementComponent::Enable()
        {
            m_enabled = true;
        }

        void MovementComponent::Disable()
        {
            m_enabled = false;
        }

        bool MovementComponent::IsEnabled()
        {
            return m_enabled;
        }

        void MovementComponent::SetPosition(const glm::dvec2& position)
        {
            m_position += position;
        }

        MovementType MovementComponent::GetMovement()
        {
            return m_movement;
        }
    }
}