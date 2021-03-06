#include "SpritesheetRenderComponent.hpp"
#include "common/Event.hpp"
#include "managers/SpritesheetManager.hpp"

namespace game
{
    namespace components
    {
        void SpritesheetRenderComponent::Create(engine::managers::EntityManager::Entity entity, SDL_Renderer* renderer, const std::string& path, const glm::dvec2& position, MovementType movementType)
        {
            RenderComponent::Create(entity, position, movementType);
            m_renderer = renderer;
            m_spriteIndex = 0;
            m_indexChangeRate = 0.0f;
            m_indexChangeRateTreshold = 0.1f;
            if (path.compare(m_path) != 0)
            {
                m_path = path;
                m_spritesheet = engine::managers::SpritesheetManager::ParseSpritesheet(path.c_str(), renderer);
            }
            m_finished = false;
            m_temporaryDisabled = false;
        }

        engine::common::Event* SpritesheetRenderComponent::Update()
        {
            engine::common::Event* message = nullptr;
            if (!m_temporaryDisabled)
            {
                if (m_finished)
                {
                    m_movement = LEFT;
                    m_spritePosition.x -= 2.0;
                    if (m_spritePosition.x <= 180.0)
                    {
                        m_enabled = false;
                        engine::common::Event* event = engine::common::EventPool::GetEvent(PLAYER_FINISHED);
                        if (event)
                        {
                            message = event->GetMessage<engine::common::PlayerFinished>();
                        }
                        else
                        {
                            message = new engine::common::PlayerFinished();
                        }
                        message->m_type = PLAYER_FINISHED;
                    }
                }
                if ((m_movement == LEFT) || (m_movement == RIGHT) || (m_movement == DESTROYED_LEFT) || (m_movement == DESTROYED_RIGHT))
                {
                    m_indexChangeRate += 0.016f;
                    engine::common::Event* messageEliminated = UpdateSpriteIndex();
                    if (!message)
                    {
                        message = messageEliminated;
                    }
                }
                if (m_enabled)
                {
                    Draw();
                }
            }
            return message;
        }

        engine::common::Event* SpritesheetRenderComponent::UpdateSpriteIndex()
        {
            engine::common::PlayerEliminated* message = nullptr;
            if (m_indexChangeRate >= m_indexChangeRateTreshold)
            {
                m_spriteIndex += 1;
                if (m_spriteIndex >= m_spritesheet[m_movement].size())
                {
                    if ((m_movement == DESTROYED_LEFT) || (m_movement == DESTROYED_RIGHT))
                    {
                        m_enabled = false;
                        engine::common::Event* event = engine::common::EventPool::GetEvent(PLAYER_ELIMINATED);
                        if (event)
                        {
                            message = event->GetMessage<engine::common::PlayerEliminated>();
                        }
                        else
                        {
                            message = new engine::common::PlayerEliminated();
                        }
                        message->m_type = PLAYER_ELIMINATED;
                    }
                    m_spriteIndex = 0;
                }
                m_indexChangeRate = 0.0f;
            }
            return message;
        }

        void SpritesheetRenderComponent::Draw()
        {
            std::pair<SDL_Texture*, glm::ivec2> spriteSettings = m_spritesheet[m_movement][m_spriteIndex];
            unsigned int yoffset = 0;
            if ((m_movement == CROUCHING_LEFT) || (m_movement == CROUCHING_RIGHT))
            {
                yoffset = 20;
            }
            SDL_Rect rect = { m_spritePosition.x, m_spritePosition.y + yoffset, spriteSettings.second.x, spriteSettings.second.y };

            //Render texture to screen
            SDL_RenderCopy(m_renderer, spriteSettings.first, NULL, &rect);
        }

        void SpritesheetRenderComponent::Receive(engine::common::Event* message)
        {
            if (message->m_type == MessageType::POSITION_CHANGE)
            {
                engine::common::ChangePoisition* changePosition = message->GetMessage<engine::common::ChangePoisition>();
                if (changePosition->m_entity == m_entity)
                {
                    switch (changePosition->m_movement)
                    {
                    case MovementType::LEFT:
                        if (m_movement != LEFT)
                        {
                            m_spriteIndex = 0;
                            m_indexChangeRate = 0.0f;
                        }
                        m_movement = LEFT;
                        break;
                    case MovementType::RIGHT:
                        if (m_movement != RIGHT)
                        {
                            m_spriteIndex = 0;
                            m_indexChangeRate = 0.0f;
                        }
                        m_movement = RIGHT;
                        break;
                    case MovementType::NO_MOVEMENT_LEFT:
                    case MovementType::NO_MOVEMENT_RIGHT:
                    case MovementType::CROUCHING_LEFT:
                    case MovementType::CROUCHING_RIGHT:
                        m_movement = changePosition->m_movement;
                        m_spriteIndex = 0;
                        break;
                    default: break;
                    }
                }
            }
            else if (message->m_type == MessageType::COLLISION_HAPPENED)
            {
                engine::common::CollisionHappened* collisionHappened = message->GetMessage<engine::common::CollisionHappened>();
                if (ComponentType::CharacterCollision == collisionHappened->m_firstColComponentType)
                {
                    switch (collisionHappened->m_secondColComponentType)
                    {
                    case ComponentType::StaticCollision:
                        if (((m_movement == RIGHT) && (collisionHappened->m_secondColDirection == DIR_LEFT)) ||
                            ((m_movement == LEFT) && (collisionHappened->m_secondColDirection == DIR_RIGHT)))
                        {
                            m_spritePosition.x = m_lastSpritePosition.x;
                        }
                        else if (collisionHappened->m_secondColDirection == DirectionType::DIR_UP)
                        {
                            m_spritePosition.y = m_lastSpritePosition.y;
                        }

                        break;
                    case ComponentType::ProjectileCollision:
                        if (m_movement == RIGHT || m_movement == NO_MOVEMENT_RIGHT)
                        {
                            m_movement = DESTROYED_LEFT;
                        }
                        else if (m_movement == LEFT || m_movement == NO_MOVEMENT_LEFT)
                        {
                            m_movement = DESTROYED_RIGHT;
                        }
                        m_spriteIndex = 0;
                        m_indexChangeRateTreshold = 0.3f;
                        break;
                    default: break;
                    }
                }
            }
            else if (message->m_type == MessageType::PLAYER_ELIMINATED)
            {
                if (m_movement == RIGHT || m_movement == NO_MOVEMENT_RIGHT)
                {
                    m_movement = DESTROYED_LEFT;
                }
                else if (m_movement == LEFT || m_movement == NO_MOVEMENT_LEFT)
                {
                    m_movement = DESTROYED_RIGHT;
                }
                m_spriteIndex = 0;
                m_indexChangeRateTreshold = 0.3f;
            }
            else if (message->m_type == MessageType::PLAYER_FINISHED)
            {
                m_finished = true;
            }
            else if (message->m_type == MessageType::PLAYER_HIDDEN)
            {
                if (m_temporaryDisabled)
                {
                    m_temporaryDisabled = false;
                }
                else
                {
                    m_temporaryDisabled = true;
                }
            }
        }

        ComponentType SpritesheetRenderComponent::GetComponentType()
        {
            return SpritesheetRender;
        }

        void SpritesheetRenderComponent::Destroy()
        {

        }
    }
}