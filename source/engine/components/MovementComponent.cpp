#include "MovementComponent.hpp"
#include <vector>

MovementComponent::MovementComponent(EntityManager::Entity entity, double positionX, double positionY, DirectionType direction) : 
    m_entity(entity), m_positionX(positionX), m_positionY(positionY), m_movement(NO_MOVEMENT), m_direction(direction)
{
}

EntityManager::Entity MovementComponent::GetEntity()
{
    return m_entity;
}

double MovementComponent::GetPositionX()
{
    return m_positionX;
}

double MovementComponent::GetPositionY()
{
    return m_positionY;
}

DirectionType MovementComponent::GetDirection()
{
    return m_direction;
}