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
        ComponentType otherCollisionType;
        DirectionType collisionDirection;
        DirectionType otherCollisionDirection;
        if (collisionComponent->CheckCollision(this, ComponentType::StaticCollision, otherCollisionType, otherCollisionDirection, collisionDirection))
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
            message->m_secondColComponentType = otherCollisionType;
            message->m_firstColDirection = collisionDirection;
            message->m_secondColDirection = otherCollisionDirection;
            message->m_type = MessageType::COLLISION_HAPPENED;
            messages.push_back(message);
        }
    }
    return messages;
}

bool StaticCollisionComponent::CheckCollision(CollisionComponent* collisionComponent, ComponentType collisionType, ComponentType& returnCollisionType, DirectionType& collisionDirection, DirectionType& collisionDirectionOther)
{
    if ((collisionType == ComponentType::CharacterCollision) || (collisionType == ComponentType::ProjectileCollision))
    {
        returnCollisionType = ComponentType::StaticCollision;
        return CollisionComponent::CheckCollision(collisionComponent, collisionType, returnCollisionType, collisionDirection, collisionDirectionOther);
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