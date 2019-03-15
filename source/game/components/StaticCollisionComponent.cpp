#include "StaticCollisionComponent.hpp"
#include "common/Event.hpp"

void StaticCollisionComponent::Create(EntityManager::Entity entity, const glm::dvec2& position, const glm::ivec2& size)
{
    CollisionComponent::Create(entity, position, size);
}

std::vector<Event*> StaticCollisionComponent::Update(const std::vector<CollisionComponent*>& collisionComponents)
{
    std::vector<Event*> messages;
    for (auto collisionComponent : collisionComponents)
    {
        DirectionType collisionDirection;
        DirectionType otherCollisionDirection;
        if (((collisionComponent->GetComponentType() == ComponentType::CharacterCollision) || (collisionComponent->GetComponentType() == ComponentType::ProjectileCollision)) &&
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
            message->m_firstColComponentType = ComponentType::StaticCollision;
            message->m_secondColEntity = collisionComponent->GetEntity();
            message->m_secondColComponentType = collisionComponent->GetComponentType();
            message->m_firstColDirection = collisionDirection;
            message->m_secondColDirection = otherCollisionDirection;
            message->m_type = MessageType::COLLISION_HAPPENED;
            messages.push_back(message);
        }
    }
    return messages;
}

bool StaticCollisionComponent::CheckCollision(CollisionComponent* collisionComponent, DirectionType& collisionDirection, DirectionType& collisionDirectionOther)
{
    if ((collisionComponent->GetComponentType() == ComponentType::CharacterCollision) || (collisionComponent->GetComponentType() == ComponentType::ProjectileCollision))
    {
        return CollisionComponent::CheckCollision(collisionComponent, collisionDirection, collisionDirectionOther);
    }
    return false;
}

void StaticCollisionComponent::Receive(Event* message)
{}

ComponentType StaticCollisionComponent::GetComponentType()
{
    return StaticCollision;
}

void StaticCollisionComponent::Destroy()
{
}