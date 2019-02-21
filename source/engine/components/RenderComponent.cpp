#include "RenderComponent.hpp"

RenderComponent::RenderComponent(EntityManager::Entity entity, double positionX, double positionY) :
    m_entity(entity), m_spritePositionX(positionX), m_spritePositionY(positionY)
{}

EntityManager::Entity RenderComponent::GetEntity()
{
    return m_entity;
}

void RenderComponent::SetPosition(double positionX, double positionY)
{
    m_spritePositionX = positionX;
    m_spritePositionY = positionY;
}

double RenderComponent::GetSpritePositionX()
{
    return m_spritePositionX;
}

double RenderComponent::GetSpritePositionY()
{
    return m_spritePositionY;
}