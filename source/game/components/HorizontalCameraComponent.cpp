#include "HorizontalCameraComponent.hpp"
#include "common/Event.hpp"

void HorizontalCameraComponent::Create(EntityManager::Entity entity, const glm::dvec2& position, const std::vector<EntityManager::Entity>& excludedEntities)
{
    CameraComponent::Create(entity, position, excludedEntities);
}

Event* HorizontalCameraComponent::Update()
{
    MoveUp* moveUp = nullptr;
    if (m_positionChange.y != 0.0)
    {
        Event* event = EventPool::GetEvent(MOVE_UP);
        if (event)
        {
            moveUp = event->GetMessage<MoveUp>();
        }
        else
        {
            moveUp = new MoveUp();
        }
        moveUp->m_type = MessageType::MOVE_UP;
        moveUp->m_moveLength = m_positionChange.y;
        moveUp->m_excludedEntities = m_excludedEntities;
        m_positionChange = glm::dvec2(0.0, 0.0);
    }
    return moveUp;
}

void HorizontalCameraComponent::Receive(Event* message)
{
    CameraComponent::Receive(message);
}

ComponentType HorizontalCameraComponent::GetComponentType()
{
    return HorizontalCamera;
}

void HorizontalCameraComponent::Destroy()
{}