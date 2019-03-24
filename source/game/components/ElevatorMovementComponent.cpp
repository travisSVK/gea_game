#include "ElevatorMovementComponent.hpp"
#include "common/Event.hpp"

namespace game
{
    namespace components
    {
        void ElevatorMovementComponent::Create(engine::managers::EntityManager::Entity entity, engine::managers::EntityManager::Entity playerEntity, const glm::dvec2& position, const glm::dvec2& playerPosition, const std::vector<engine::managers::EntityManager::Entity>& excludedEntities)
        {
            MovementComponent::Create(entity, position, DirectionType::DIR_LEFT);
            m_playerEntity = playerEntity;
            m_playerPosition = playerPosition;
            m_move = false;
            m_excludedEntities = excludedEntities;
        }

        std::vector<engine::common::Event*> ElevatorMovementComponent::Update()
        {
            std::vector<engine::common::Event*> messages;
            if (m_move)
            {
                engine::common::MoveUp* moveUp;
                engine::common::Event* event = engine::common::EventPool::GetEvent(MOVE_UP);
                if (event)
                {
                    moveUp = event->GetMessage<engine::common::MoveUp>();
                }
                else
                {
                    moveUp = new engine::common::MoveUp();
                }
                moveUp->m_moveLength = 2.0f;
                moveUp->m_type = MOVE_UP;
                moveUp->m_excludedEntities = m_excludedEntities;
                messages.push_back(moveUp);
            }
            else if (m_position.y != 135.0)
            {
                // move the elevator to the next position
                float moveLength;
                if ((m_position.y + 2.0) > 135.0)
                {
                    moveLength = 135.0 - m_position.y;
                    m_position.y = 135.0;
                }
                else
                {
                    moveLength = 2.0;
                    m_position.y += 2.0;
                }

                engine::common::MoveElevator* moveElevator;
                engine::common::Event* event = engine::common::EventPool::GetEvent(MOVE_ELEVATOR);
                if (event)
                {
                    moveElevator = event->GetMessage<engine::common::MoveElevator>();
                }
                else
                {
                    moveElevator = new engine::common::MoveElevator();
                }
                moveElevator->m_moveLength = moveLength;
                moveElevator->m_type = MOVE_ELEVATOR;
                std::vector<engine::managers::EntityManager::Entity> moveEntities;
                for (auto entity : m_excludedEntities)
                {
                    if (entity != m_playerEntity)
                    {
                        moveEntities.push_back(entity);
                    }
                }
                moveElevator->m_elevatorEntities = moveEntities;
                messages.push_back(moveElevator);
            }
            return messages;
        }
        void ElevatorMovementComponent::Receive(engine::common::Event* message)
        {
            if (message->m_type == MessageType::POSITION_CHANGE)
            {
                engine::common::ChangePoisition* changePosition = message->GetMessage<engine::common::ChangePoisition>();
                m_playerPosition = changePosition->m_absolutePosition;
            }

            if (message->m_type == MessageType::MOVE)
            {
                engine::common::Move* move = message->GetMessage<engine::common::Move>();
                if ((move->m_movement == DOWN) && IsPlayerWithinBounds())
                {
                    m_move = true;
                }
                else if (move->m_movement == NO_MOVEMENT)
                {
                    m_move = false;
                }
            }
        }

        bool ElevatorMovementComponent::IsPlayerWithinBounds()
        {
            if ((m_playerPosition.x >= 378.0) && (m_playerPosition.x < 402.0) && ((m_playerPosition.y + 30) == m_position.y))
            {
                return true;
            }
            return false;
        }

        ComponentType ElevatorMovementComponent::GetComponentType()
        {
            return ElevatorMovement;
        }

        void ElevatorMovementComponent::Destroy()
        {
        }
    }
}