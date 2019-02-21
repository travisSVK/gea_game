#include "ProjectileMovementComponent.hpp"

ProjectileMovementComponent::ProjectileMovementComponent(EntityManager::Entity entity, double positionX, double positionY, DirectionType direction):
    MovementComponent(entity, positionX, positionY, direction)
{
}

bool ProjectileMovementComponent::Update(float dt, double& positionX, double& positionY)
{
    if (m_direction == DIR_LEFT)
    {
        m_positionX -= dt * 150;
    }
    else if (m_direction == DIR_RIGHT)
    {
        m_positionX += dt * 150;
    }
    positionX = m_positionX;
    positionY = m_positionY;
    return true;
}

void ProjectileMovementComponent::Receive(Message* message)
{
}