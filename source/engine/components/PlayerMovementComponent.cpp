#include "PlayerMovementComponent.hpp"
#include "../common/Event.hpp"

PlayerMovementComponent::PlayerMovementComponent(EntityManager::Entity entity, double positionX, double positionY, DirectionType direction) :
    MovementComponent(entity, positionX, positionY, direction)
{
}

bool PlayerMovementComponent::Update(float dt, double& positionX, double& positionY)
{
    if (m_movement == NO_MOVEMENT)
    {
        return false;
    }
    else if (m_movement == LEFT)
    {
        m_positionX -= dt * 100;
    }
    else if (m_movement == RIGHT)
    {
        m_positionX += dt * 100;
    }
    
    positionX = m_positionX;
    positionY = m_positionY;
    return true;
}

void PlayerMovementComponent::Receive(Message* message)
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
                m_direction = DirectionType::DIR_LEFT;
            }
            else if (m_movement == MovementType::RIGHT)
            {
                m_direction = DirectionType::DIR_RIGHT;
            }
            m_movement = MovementType::NO_MOVEMENT;
            break;

        default: break;
        }
    }
}