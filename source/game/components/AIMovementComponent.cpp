#include "AIMovementComponent.hpp"
#include "common/Event.hpp"

namespace game
{
    namespace components
    {
        void AIMovementComponent::Create(engine::managers::EntityManager::Entity entity, const glm::dvec2& position, const glm::dvec2& playerPosition, DirectionType direction, engine::managers::EntityManager* entityManager)
        {
            MovementComponent::Create(entity, position, direction);
            m_playerPosition = playerPosition;
            m_entityManager = entityManager;
            m_reloadTime = 0.0f;
            if (direction == DIR_LEFT)
            {
                m_movement = LEFT;
            }
            else if (direction == DIR_RIGHT)
            {
                m_movement = RIGHT;
            }
            if (position.x < 370.0)
            {
                m_leftBound = 150.0;
                m_rightBound = 350.0;
            }
            else
            {
                m_leftBound = 430.0;
                m_rightBound = 630.0;
            }
        }

        std::vector<engine::common::Event*> AIMovementComponent::Update()
        {
            std::vector<engine::common::Event*> messages;
            if (((m_playerPosition.y <= m_position.y + 40) && (m_playerPosition.y >= m_position.y - 30)) && (m_reloadTime >= 3.0))
            {
                if (((m_playerPosition.x < m_position.x) && (m_direction == DIR_LEFT)) ||
                    ((m_playerPosition.x > m_position.x) && (m_direction == DIR_RIGHT)))
                {
                    engine::common::CreateProjectile* createProjectile;
                    engine::common::Event* message = engine::common::EventPool::GetEvent(CREATE_PROJECTILE);
                    if (message)
                    {
                        createProjectile = message->GetMessage<engine::common::CreateProjectile>();
                    }
                    else
                    {
                        createProjectile = new engine::common::CreateProjectile();
                    }

                    createProjectile->m_newEntity = m_entityManager->CreateEntity();
                    createProjectile->m_projetileSprites = "data/sprite/projectile/projectile.json";
                    createProjectile->m_type = CREATE_PROJECTILE;
                    messages.push_back(createProjectile);
                    m_reloadTime = 0.0f;
                }
            }

            glm::dvec2 positionChange(0.0, 0.0);
            m_reloadTime += 0.016f;
            m_lastPosition = m_position;
            if (m_movement == LEFT)
            {
                if ((m_position.x - 2.0) <= m_leftBound)
                {
                    positionChange.x -= m_position.x - m_leftBound;
                    m_position.x = m_leftBound;
                    m_movement = RIGHT;
                    m_direction = DIR_RIGHT;
                }
                else
                {
                    m_position.x -= 2.0;
                    positionChange.x -= 2.0;
                }
            }
            else if (m_movement == RIGHT)
            {
                if ((m_position.x + 4.0) >= m_rightBound)
                {
                    positionChange.x = m_rightBound - m_position.x;
                    m_position.x = m_rightBound;
                    m_movement = LEFT;
                    m_direction = DIR_LEFT;
                }
                else
                {
                    m_position.x += 2.0;
                    positionChange.x += 2.0;
                }
            }

            engine::common::ChangePoisition* changePositionMessage;
            engine::common::Event* message = engine::common::EventPool::GetEvent(POSITION_CHANGE);
            if (message)
            {
                changePositionMessage = message->GetMessage<engine::common::ChangePoisition>();
            }
            else
            {
                changePositionMessage = new engine::common::ChangePoisition();
            }
            changePositionMessage->m_position = positionChange;
            changePositionMessage->m_absolutePosition = m_position;
            changePositionMessage->m_movement = m_movement;
            changePositionMessage->m_type = MessageType::POSITION_CHANGE;
            messages.push_back(changePositionMessage);
            return messages;
        }

        void AIMovementComponent::Receive(engine::common::Event* message)
        {
            if (message->m_type == MessageType::COLLISION_HAPPENED)
            {
                engine::common::CollisionHappened* collisionHappened = message->GetMessage<engine::common::CollisionHappened>();
                if (ComponentType::CharacterCollision == collisionHappened->m_firstColComponentType)
                {
                    switch (collisionHappened->m_secondColComponentType)
                    {
                    case ComponentType::StaticCollision:
                        if (((m_movement == LEFT) && (collisionHappened->m_secondColDirection == DirectionType::DIR_RIGHT))
                            || ((m_movement == RIGHT) && (collisionHappened->m_secondColDirection == DirectionType::DIR_LEFT)))
                        {
                            m_position.x = m_lastPosition.x;
                        }
                        break;
                    case ComponentType::ProjectileCollision:
                        m_enabled = false;
                        break;
                    default: break;
                    }
                }
            }
            else if (message->m_type == MessageType::POSITION_CHANGE)
            {
                engine::common::ChangePoisition* changePosition = message->GetMessage<engine::common::ChangePoisition>();
                m_playerPosition = changePosition->m_absolutePosition;
            }
        }

        ComponentType AIMovementComponent::GetComponentType()
        {
            return AIMovement;
        }

        void AIMovementComponent::Destroy()
        {}
    }
}