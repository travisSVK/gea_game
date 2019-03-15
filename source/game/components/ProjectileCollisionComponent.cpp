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
        if ((collisionComponent->GetEntity() != m_spawnedFromEntity) && 
            ((collisionComponent->GetComponentType() != ComponentType::CharacterCollision) || (collisionComponent->GetEntity() == m_targetEntity) || (m_targetEntity == -1)) &&
            ((collisionComponent->GetComponentType() == ComponentType::StaticCollision) || (collisionComponent->GetComponentType() == ComponentType::CharacterCollision)) &&
            collisionComponent->CheckCollision(this, otherCollisionDirection, collisionDirection))
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
            message->m_secondColComponentType = collisionComponent->GetComponentType();
            message->m_firstColDirection = collisionDirection;
            message->m_secondColDirection = otherCollisionDirection;
            message->m_type = MessageType::COLLISION_HAPPENED;
            messages.push_back(message);
            m_enabled = false;
        }
    }
    return messages;
}

bool ProjectileCollisionComponent::CheckCollision(CollisionComponent* collisionComponent, DirectionType& collisionDirection, DirectionType& collisionDirectionOther)
{
    if (m_enabled && ((collisionComponent->GetComponentType() != ComponentType::CharacterCollision) || (collisionComponent->GetEntity() == m_targetEntity) || (m_targetEntity == -1)) &&
        (m_spawnedFromEntity != collisionComponent->GetEntity()) && ((collisionComponent->GetComponentType() == ComponentType::StaticCollision) || (collisionComponent->GetComponentType() == ComponentType::CharacterCollision)))
    {
        if (CollisionComponent::CheckCollision(collisionComponent, collisionDirection, collisionDirectionOther))
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