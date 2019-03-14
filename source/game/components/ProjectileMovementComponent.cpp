#include "ProjectileMovementComponent.hpp"
#include "common/Event.hpp"

void ProjectileMovementComponent::Create(EntityManager::Entity entity, const glm::dvec2& position, DirectionType direction)
{
    MovementComponent::Create(entity, position, direction);
}

std::vector<Event*> ProjectileMovementComponent::Update()
{
    std::vector<Event*> messages;
    glm::dvec2 positionChange(0.0, 0.0);
    if (m_direction == DirectionType::DIR_LEFT)
    {
        m_position.x -= 2.5;
        positionChange.x -= 2.5;
    }
    else if (m_direction == DirectionType::DIR_RIGHT)
    {
        m_position.x += 2.5;
        positionChange.x += 2.5;
    }
    ChangePoisition* changePositionMessage;
    Event* event = EventPool::GetEvent(POSITION_CHANGE);
    if (event)
    {
        changePositionMessage = event->GetMessage<ChangePoisition>();
    }
    else
    {
        changePositionMessage = new ChangePoisition();
    }
    changePositionMessage->m_position = positionChange;
    changePositionMessage->m_type = MessageType::POSITION_CHANGE;
    messages.push_back(changePositionMessage);
    return messages;
}

void ProjectileMovementComponent::Receive(Event* message)
{
    if (message->m_type == MessageType::COLLISION_HAPPENED)
    {
        CollisionHappened* collisionHappened = message->GetMessage<CollisionHappened>();
        if ((ComponentType::ProjectileCollision == collisionHappened->m_firstColComponentType) && (ComponentType::ProjectileCollision != collisionHappened->m_secondColComponentType))
        {
            m_enabled = false;
        }
    }
}

ComponentType ProjectileMovementComponent::GetComponentType()
{
    return ProjectileMovement;
}

void ProjectileMovementComponent::Destroy()
{

}