#include "CollisionComponent.hpp"

void CollisionComponent::Create(EntityManager::Entity entity, const glm::dvec2& position, const glm::ivec2& size)
{
    m_entity = entity;
    m_size = size;
    m_enabled = true;
    m_position = glm::dvec2(position.x + (static_cast<double>(size.x) / 2.0), position.y + (static_cast<double>(size.y) / 2.0));
    m_lastPosition = m_position;
}

bool CollisionComponent::CheckCollision(CollisionComponent* collisionComponent, ComponentType collisionType, ComponentType& returnCollisionType, DirectionType& collisionDirection, DirectionType& collisionDirectionOther)
{
    glm::dvec2 position = collisionComponent->GetPosition();
    glm::ivec2 size = collisionComponent->GetSize();
    double distanceX = glm::abs(position.x - m_position.x);
    double distanceY = glm::abs(position.y - m_position.y);
    double widthSum = (static_cast<double>(size.x) / 2.0) + static_cast<double>(m_size.x) / 2.0;
    double heightSum = (static_cast<double>(size.y) / 2.0) + static_cast<double>(m_size.y) / 2.0;

    
    if ((distanceX < widthSum) && (distanceY < heightSum))
    {
        if ((widthSum - distanceX) >= (heightSum - distanceY))
        {
            if (m_position.y > position.y)
            {
                collisionDirection = DIR_UP;
                collisionDirectionOther = DIR_DOWN;
            }
            else
            {
                collisionDirection = DIR_DOWN;
                collisionDirectionOther = DIR_UP;
            }
        }
        else
        {
            if (m_position.x > position.x)
            {
                collisionDirection = DIR_LEFT;
                collisionDirectionOther = DIR_RIGHT;
            }
            else
            {
                collisionDirection = DIR_RIGHT;
                collisionDirectionOther = DIR_LEFT;
            }
        }
        return true;
    }
    return false;
}

void CollisionComponent::SetPosition(const glm::dvec2& position)
{
    m_position = glm::dvec2(position.x + (static_cast<double>(m_size.x) / 2.0), position.y + (static_cast<double>(m_size.y) / 2.0));;
}

void CollisionComponent::SetCenterPosition(const glm::dvec2& position)
{
    if (GetComponentType() == StaticCollision)
    {
        m_lastPosition = m_position;
        m_lastPosition.x += 0;
    }
    m_lastPosition = m_position;
    m_position += position;
}

EntityManager::Entity CollisionComponent::GetEntity()
{
    return m_entity;
}

glm::dvec2 CollisionComponent::GetPosition()
{
    return m_position;
}

glm::ivec2 CollisionComponent::GetSize()
{
    return m_size;
}

void CollisionComponent::Enable()
{
    m_enabled = true;
}

void CollisionComponent::Disable()
{
    m_enabled = false;
}

bool CollisionComponent::IsEnabled()
{
    return m_enabled;
}