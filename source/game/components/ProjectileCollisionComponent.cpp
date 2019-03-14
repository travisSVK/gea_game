#include "ProjectileCollisionComponent.hpp"
#include "common/Event.hpp"

void ProjectileCollisionComponent::Create(EntityManager::Entity entity,
    EntityManager::Entity spawnedFromEntity,
    int targetEntity,
    const glm::dvec2& position, const glm::ivec2& size)
{
    CollisionComponent::Create(entity, position, size);
    m_spawnedFromEntity = spawnedFromEntity;
    m_targetEntity = targetEntity;
}

std::vector<Event*> ProjectileCollisionComponent::Update(const std::vector<CollisionComponent*>& collisionComponents)
{
    std::vector<Event*> messages;
    for (auto collisionComponent : collisionComponents)
    {
        DirectionType collisionDirection;
        DirectionType otherCollisionDirection;
        ComponentType otherCollisionType;
        if ((collisionComponent->GetEntity() != m_spawnedFromEntity) && 
            collisionComponent->CheckCollision(this, ComponentType::ProjectileCollision, otherCollisionType, otherCollisionDirection, collisionDirection))
        {
            if ((otherCollisionType != ComponentType::CharacterCollision) || ((m_targetEntity != -1) && (collisionComponent->GetEntity() == m_targetEntity)) || (m_targetEntity == -1))
            {
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
                message->m_firstColComponentType = ComponentType::ProjectileCollision;
                message->m_secondColEntity = collisionComponent->GetEntity();
                message->m_secondColComponentType = otherCollisionType;
                message->m_firstColDirection = collisionDirection;
                message->m_secondColDirection = otherCollisionDirection;
                message->m_type = MessageType::COLLISION_HAPPENED;
                messages.push_back(message);
                m_enabled = false;
            }
        }
    }
    return messages;
}

bool ProjectileCollisionComponent::CheckCollision(CollisionComponent* collisionComponent, ComponentType collisionType, ComponentType& returnCollisionType, DirectionType& collisionDirection, DirectionType& collisionDirectionOther)
{
    if (m_enabled && ((collisionType != ComponentType::CharacterCollision) || ((m_targetEntity != -1) && (collisionComponent->GetEntity() == m_targetEntity)) || (m_targetEntity == -1)) &&
        (m_spawnedFromEntity != collisionComponent->GetEntity()) && (collisionType == ComponentType::StaticCollision) || (collisionType == ComponentType::CharacterCollision))
    {
        returnCollisionType = ComponentType::ProjectileCollision;
        if (CollisionComponent::CheckCollision(collisionComponent, collisionType, returnCollisionType, collisionDirection, collisionDirectionOther))
        {
            m_enabled = false;
            return true;
        }
    }
    return false;
}

void ProjectileCollisionComponent::Receive(Event* message)
{}

ComponentType ProjectileCollisionComponent::GetComponentType()
{
    return ProjectileCollision;
}

void ProjectileCollisionComponent::Destroy()
{}