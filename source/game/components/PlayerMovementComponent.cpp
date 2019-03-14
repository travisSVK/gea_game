#include "PlayerMovementComponent.hpp"
#include "common/Event.hpp"

void PlayerMovementComponent::Create(EntityManager::Entity entity, const glm::dvec2& position, DirectionType direction)
{
    MovementComponent::Create(entity, position, direction);
    m_lastPosition = position;
    m_collisionUp = false;
}

std::vector<Event*> PlayerMovementComponent::Update()
{
    std::vector<Event*> messages;
    glm::dvec2 positionChange(0.0, 0.0);
    m_lastPosition = m_position;
    if (!(((m_position.x >= 352.0) && (m_position.x <= 378.0)) || ((m_position.x >= 402.0) && (m_position.x <= 428.0))))
    {
        if (!(m_collisionUp && ((m_movement == NO_MOVEMENT_LEFT) || (m_movement == NO_MOVEMENT_RIGHT) || (m_movement == NO_MOVEMENT))))
        {
            positionChange.y += 1.0;
        }
    }
    if (m_collisionUp && (m_movement != NO_MOVEMENT_LEFT) && (m_movement != NO_MOVEMENT_RIGHT) && (m_movement != NO_MOVEMENT))
    {
        m_collisionUp = false;
    }
    
    if (m_movement == LEFT)
    {
        m_position.x -= 2.0;
        positionChange.x -= 2.0;
    }
    else if (m_movement == RIGHT)
    {
        m_position.x += 2.0;
        positionChange.x += 2.0;
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
    changePositionMessage->m_absolutePosition = m_position;
    changePositionMessage->m_movement = m_movement;
    changePositionMessage->m_type = MessageType::POSITION_CHANGE;
    messages.push_back(changePositionMessage);
    return messages;
}

void PlayerMovementComponent::Receive(Event* message)
{
    if (message->m_type == MessageType::MOVE)
    {
        Move* move = message->GetMessage<Move>();
        switch (move->m_movement)
        {
        case MovementType::LEFT:
            m_movement = MovementType::LEFT;
            m_direction = DirectionType::DIR_LEFT;
            break;
        case MovementType::RIGHT:
            m_movement = MovementType::RIGHT;
            m_direction = DirectionType::DIR_RIGHT;
            break;
        case MovementType::NO_MOVEMENT:
            if (m_movement == MovementType::LEFT)
            {
                m_movement = MovementType::NO_MOVEMENT_LEFT;
                m_direction = DirectionType::DIR_LEFT;
            }
            else if (m_movement == MovementType::RIGHT)
            {
                m_movement = MovementType::NO_MOVEMENT_RIGHT;
                m_direction = DirectionType::DIR_RIGHT;
            }
            break;

        default: break;
        }
    }
    else if (message->m_type == MessageType::COLLISION_HAPPENED)
    {
        CollisionHappened* collisionHappened = message->GetMessage<CollisionHappened>();
        if (ComponentType::CharacterCollision == collisionHappened->m_firstColComponentType)
        {
            switch (collisionHappened->m_secondColComponentType)
            {
            case ComponentType::StaticCollision:
                if (((m_movement == LEFT) && (collisionHappened->m_secondColDirection == DirectionType::DIR_RIGHT))
                    || ((m_movement == RIGHT) && (collisionHappened->m_secondColDirection == DirectionType::DIR_LEFT)))
                {
                    m_position.x = m_lastPosition.x;
                }
                else if (collisionHappened->m_secondColDirection == DirectionType::DIR_UP)
                {
                    m_position.y = m_lastPosition.y;
                    m_collisionUp = true;
                }
                break;
            case ComponentType::ProjectileCollision:
                m_enabled = false;
                break;
            default: break;
            }
        }
    }
}

ComponentType PlayerMovementComponent::GetComponentType()
{
    return PlayerMovement;
}

void PlayerMovementComponent::Destroy()
{
}