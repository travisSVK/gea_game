#include "StaticCollisionComponent.hpp"
#include "common/Event.hpp"

namespace game
{
    namespace components
    {
        void StaticCollisionComponent::Create(engine::managers::EntityManager::Entity entity, const glm::dvec2& position, const glm::ivec2& size)
        {
            CollisionComponent::Create(entity, position, size);
        }

        std::vector<engine::common::Event*> StaticCollisionComponent::Update(const std::vector<CollisionComponent*>& collisionComponents)
        {
            std::vector<engine::common::Event*> messages;
            for (auto collisionComponent : collisionComponents)
            {
                DirectionType collisionDirection;
                DirectionType otherCollisionDirection;
                if (((collisionComponent->GetComponentType() == ComponentType::CharacterCollision) || (collisionComponent->GetComponentType() == ComponentType::ProjectileCollision)) &&
                    collisionComponent->CheckCollision(this, otherCollisionDirection, collisionDirection))
                {
                    engine::common::CollisionHappened* message;
                    engine::common::Event* event = engine::common::EventPool::GetEvent(COLLISION_HAPPENED);
                    if (event)
                    {
                        message = event->GetMessage<engine::common::CollisionHappened>();
                    }
                    else
                    {
                        message = new engine::common::CollisionHappened();
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

        void StaticCollisionComponent::Receive(engine::common::Event* message)
        {}

        ComponentType StaticCollisionComponent::GetComponentType()
        {
            return StaticCollision;
        }

        void StaticCollisionComponent::Destroy()
        {
        }
    }
}