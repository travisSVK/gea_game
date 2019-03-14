#include "ElevatorMovementComponent.hpp"
#include "common/Event.hpp"

void ElevatorMovementComponent::Create(EntityManager::Entity entity, EntityManager::Entity playerEntity, const glm::dvec2& position, const glm::dvec2& playerPosition, const std::vector<EntityManager::Entity>& excludedEntities)
{
    MovementComponent::Create(entity, position, DirectionType::DIR_LEFT);
    m_playerEntity = playerEntity;
    m_playerPosition = playerPosition;
    m_move = false;
    m_excludedEntities = excludedEntities;
}

std::vector<Event*> ElevatorMovementComponent::Update()
{
    std::vector<Event*> messages;
    if (m_move)
    {
        MoveUp* moveUp;
        Event* event = EventPool::GetEvent(MOVE_UP);
        if (event)
        {
            moveUp = event->GetMessage<MoveUp>();
        }
        else
        {
            moveUp = new MoveUp();
        }
        moveUp->m_moveLength = 2.0f;
        moveUp->m_type = MOVE_UP;
        moveUp->m_excludedEntities = m_excludedEntities;
        messages.push_back(moveUp);
    }
    else if(m_position.y != 135.0)
    {
        // move the elevator to the next position
        float moveLength;
        if ((m_position.y + 2.0) > 135.0)
        {
            moveLength = 135.0 - m_position.y;
            m_position.y = 135.0;
        }
        else
        {
            moveLength = 2.0;
            m_position.y += 2.0;
        }
        
        MoveElevator* moveElevator;
        Event* event = EventPool::GetEvent(MOVE_ELEVATOR);
        if (event)
        {
            moveElevator = event->GetMessage<MoveElevator>();
        }
        else
        {
            moveElevator = new MoveElevator();
        }
        moveElevator->m_moveLength = moveLength;
        moveElevator->m_type = MOVE_ELEVATOR;
        std::vector<EntityManager::Entity> moveEntities;
        for (auto entity : m_excludedEntities)
        {
            if (entity != m_playerEntity)
            {
                moveEntities.push_back(entity);
            }
        }
        moveElevator->m_elevatorEntities = moveEntities;
        messages.push_back(moveElevator);
    }
    return messages;
}
void ElevatorMovementComponent::Receive(Event* message)
{
    if (message->m_type == MessageType::POSITION_CHANGE)
    {
        ChangePoisition* changePosition = message->GetMessage<ChangePoisition>();
        m_playerPosition = changePosition->m_absolutePosition;
    }

    if (message->m_type == MessageType::MOVE)
    {
        Move* move = message->GetMessage<Move>();
        if ((move->m_movement == DOWN) && IsPlayerWithinBounds())
        {
            m_move = true;
        }
        else if (move->m_movement == NO_MOVEMENT)
        {
            m_move = false;
        }
    }
}

bool ElevatorMovementComponent::IsPlayerWithinBounds()
{
    if ((m_playerPosition.x >= 378.0) && (m_playerPosition.x < 402.0) && ((m_playerPosition.y + 30) == m_position.y))
    {
        return true;
    }
    return false;
}

ComponentType ElevatorMovementComponent::GetComponentType()
{
    return ElevatorMovement;
}

void ElevatorMovementComponent::Destroy()
{
}