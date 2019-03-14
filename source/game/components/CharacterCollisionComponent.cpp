#include "CharacterCollisionComponent.hpp"
#include "common/Event.hpp"

void CharacterCollisionComponent::Create(EntityManager::Entity entity, const glm::dvec2& position, const glm::ivec2& size, bool isPlayerEntity)
{
    CollisionComponent::Create(entity, position, size);
    m_isPlayerEntity = isPlayerEntity;
}

std::vector<Event*> CharacterCollisionComponent::Update(const std::vector<CollisionComponent*>& collisionComponents)
{
    std::vector<Event*> messages;
    for (auto collisionComponent : collisionComponents)
    {
        DirectionType collisionDirection;
        DirectionType otherCollisionDirection;
        ComponentType otherCollisionType;
        if (collisionComponent->CheckCollision(this, ComponentType::CharacterCollision, otherCollisionType, otherCollisionDirection, collisionDirection))
        {
            if ((otherCollisionType == ComponentType::ProjectileCollision) && (!m_isPlayerEntity))
            {
                m_enabled = false;
            }
            else if (otherCollisionType == ComponentType::StaticCollision)
            {
                if ((otherCollisionDirection == DirectionType::DIR_LEFT) || (otherCollisionDirection == DirectionType::DIR_RIGHT))
                {
                    m_position.x = m_lastPosition.x;
                }
                else if (otherCollisionDirection == DirectionType::DIR_UP)
                {
                    m_position.y = m_lastPosition.y;
                }
            }
            CollisionHappened* message;
            Event* event = EventPool::GetEvent(COLLISION_HAPPENED);
            if (event)
            {
                message = event->GetMessage<CollisionHappened>();
            }
            else
            {
                message = new CollisionHappened();
            }
            message->m_firstColEntity = m_entity;
            message->m_firstColComponentType = ComponentType::CharacterCollision;
            message->m_secondColEntity = collisionComponent->GetEntity();
            message->m_secondColComponentType = otherCollisionType;
            message->m_firstColDirection = collisionDirection;
            message->m_secondColDirection = otherCollisionDirection;
            message->m_type = MessageType::COLLISION_HAPPENED;
            messages.push_back(message);
        }
    }
    if (m_isPlayerEntity)
    {
        m_position.y = m_lastPosition.y;
    }
    return messages;
}

bool CharacterCollisionComponent::CheckCollision(CollisionComponent* collisionComponent, ComponentType collisionType, ComponentType& returnCollisionType, DirectionType& collisionDirection, DirectionType& collisionDirectionOther)
{
    if (m_enabled && ((collisionType == ComponentType::StaticCollision) || (collisionType == ComponentType::ProjectileCollision)))
    {
        returnCollisionType = ComponentType::CharacterCollision;
        if (CollisionComponent::CheckCollision(collisionComponent, collisionType, returnCollisionType, collisionDirection, collisionDirectionOther))
        {
            if ((collisionType == ComponentType::ProjectileCollision) && (!m_isPlayerEntity))
            {
                m_enabled = false;
            }
            else if (collisionType == ComponentType::StaticCollision)
            {
                if ((collisionDirectionOther == DirectionType::DIR_LEFT) || (collisionDirectionOther == DirectionType::DIR_RIGHT))
                {
                    m_position.x = m_lastPosition.x;
                }
                else if (collisionDirectionOther == DirectionType::DIR_UP)
                {
                    m_position.y = m_lastPosition.y;
                }
            }
            return true;
        }
    }
    return false;
}

void CharacterCollisionComponent::Receive(Event* message)
{
}

ComponentType CharacterCollisionComponent::GetComponentType()
{
    return CharacterCollision;
}

void CharacterCollisionComponent::Destroy()
{

}