#include "CameraComponent.hpp"
#include "../common/Event.hpp"
#include <algorithm>

void CameraComponent::Create(EntityManager::Entity entity, const glm::dvec2& position, const std::vector<EntityManager::Entity>& excludedEntities)
{
    m_entity = entity;
    m_position = position;
    m_excludedEntities = excludedEntities;
    m_positionChange = glm::dvec2(0.0, 0.0);
    m_enabled = true;
}

void CameraComponent::Receive(Event* message)
{
    if (message->m_type == MessageType::COLLISION_HAPPENED)
    {
        CollisionHappened* collisionHappened = message->GetMessage<CollisionHappened>();
        if ((ComponentType::CharacterCollision == collisionHappened->m_firstColComponentType) && (ComponentType::StaticCollision == collisionHappened->m_secondColComponentType))
        {
            m_position -= m_positionChange;
            m_positionChange = glm::dvec2(0.0, 0.0);
        }
    }
}

void CameraComponent::SetPosition(glm::dvec2 position)
{
    m_positionChange = position;
    m_position += position;
}

EntityManager::Entity CameraComponent::GetEntity()
{
    return m_entity;
}

void CameraComponent::Enable()
{
    m_enabled = true;
}

void CameraComponent::Disable()
{
    m_enabled = false;
}

bool CameraComponent::IsEnabled()
{
    return m_enabled;
}