#include "ProjectileMovementComponent.hpp"
#include "common/Event.hpp"

namespace game
{
    namespace components
    {
        void ProjectileMovementComponent::Create(engine::managers::EntityManager::Entity entity, const glm::dvec2& position, DirectionType direction)
        {
            MovementComponent::Create(entity, position, direction);
        }

        std::vector<engine::common::Event*> ProjectileMovementComponent::Update()
        {
            std::vector<engine::common::Event*> messages;
            glm::dvec2 positionChange(0.0, 0.0);
            if (m_direction == DirectionType::DIR_LEFT)
            {
                m_position.x -= 2.5;
                positionChange.x -= 2.5;
            }
            else if (m_direction == DirectionType::DIR_RIGHT)
            {
                m_position.x += 2.5;
                positionChange.x += 2.5;
            }
            engine::common::ChangePoisition* changePositionMessage;
            engine::common::Event* event = engine::common::EventPool::GetEvent(POSITION_CHANGE);
            if (event)
            {
                changePositionMessage = event->GetMessage<engine::common::ChangePoisition>();
            }
            else
            {
                changePositionMessage = new engine::common::ChangePoisition();
            }
            changePositionMessage->m_position = positionChange;
            changePositionMessage->m_type = MessageType::POSITION_CHANGE;
            messages.push_back(changePositionMessage);
            return messages;
        }

        void ProjectileMovementComponent::Receive(engine::common::Event* message)
        {
            if (message->m_type == MessageType::COLLISION_HAPPENED)
            {
                engine::common::CollisionHappened* collisionHappened = message->GetMessage<engine::common::CollisionHappened>();
                if ((ComponentType::ProjectileCollision == collisionHappened->m_firstColComponentType) && (ComponentType::ProjectileCollision != collisionHappened->m_secondColComponentType))
                {
                    m_enabled = false;
                }
            }
        }

        ComponentType ProjectileMovementComponent::GetComponentType()
        {
            return ProjectileMovement;
        }

        void ProjectileMovementComponent::Destroy()
        {

        }
    }
}