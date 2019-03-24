#include "ProjectileCollisionComponent.hpp"
#include "common/Event.hpp"

namespace game
{
    namespace components
    {
        void ProjectileCollisionComponent::Create(engine::managers::EntityManager::Entity entity,
            engine::managers::EntityManager::Entity spawnedFromEntity,
            int targetEntity,
            const glm::dvec2& position, const glm::ivec2& size)
        {
            CollisionComponent::Create(entity, position, size);
            m_spawnedFromEntity = spawnedFromEntity;
            m_targetEntity = targetEntity;
        }

        std::vector<engine::common::Event*> ProjectileCollisionComponent::Update(const std::vector<CollisionComponent*>& collisionComponents)
        {
            std::vector<engine::common::Event*> messages;
            for (auto collisionComponent : collisionComponents)
            {
                DirectionType collisionDirection;
                DirectionType otherCollisionDirection;
                if ((collisionComponent->GetEntity() != m_spawnedFromEntity) &&
                    ((collisionComponent->GetComponentType() != ComponentType::CharacterCollision) || (collisionComponent->GetEntity() == m_targetEntity) || (m_targetEntity == -1)) &&
                    ((collisionComponent->GetComponentType() == ComponentType::StaticCollision) || (collisionComponent->GetComponentType() == ComponentType::CharacterCollision)) &&
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

        void ProjectileCollisionComponent::Receive(engine::common::Event* message)
        {}

        ComponentType ProjectileCollisionComponent::GetComponentType()
        {
            return ProjectileCollision;
        }

        void ProjectileCollisionComponent::Destroy()
        {}
    }
}