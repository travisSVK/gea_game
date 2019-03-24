#include "SpriteRenderComponent.hpp"
#include "common/Event.hpp"
#include "managers/SpritesheetManager.hpp"

namespace game
{
    namespace components
    {
        void SpriteRenderComponent::Create(engine::managers::EntityManager::Entity entity, SDL_Renderer* renderer, const std::string& path, const glm::dvec2& position, MovementType movementType)
        {
            RenderComponent::Create(entity, position, movementType);
            m_renderer = renderer;
            m_destroyedChangeRate = 0.0f;
            m_destroyedChangeRateTreshold = 0.0f;
            if (path.compare(m_path) != 0)
            {
                m_path = path;
                m_sprites = engine::managers::SpritesheetManager::ParseSprites(path.c_str(), renderer);
            }
        }

        engine::common::Event* SpriteRenderComponent::Update()
        {
            engine::common::Event* message = nullptr;
            if ((m_movement == DESTROYED_LEFT) || (m_movement == DESTROYED_RIGHT))
            {
                m_destroyedChangeRate += 0.016f;
                if (m_destroyedChangeRate >= m_destroyedChangeRateTreshold)
                {
                    m_enabled = false;
                }
            }
            else if (m_movement == CAR_DRIVE)
            {
                m_spritePosition.x += 2.0;
                if (m_spritePosition.x >= 800.0)
                {
                    m_enabled = false;
                    engine::common::DriveCar* driveCar;
                    engine::common::Event* event = engine::common::EventPool::GetEvent(DRIVE_CAR);
                    if (event)
                    {
                        driveCar = event->GetMessage<engine::common::DriveCar>();
                    }
                    else
                    {
                        driveCar = new engine::common::DriveCar();
                    }
                    driveCar->m_type = DRIVE_CAR;
                    message = driveCar;
                }
            }
            Draw();
            return message;
        }

        void SpriteRenderComponent::Draw()
        {
            std::pair<SDL_Texture*, glm::ivec2> spriteSettings = m_sprites[m_movement];
            SDL_Rect rect = { m_spritePosition.x, m_spritePosition.y, spriteSettings.second.x, spriteSettings.second.y };

            //Render texture to screen
            SDL_RenderCopy(m_renderer, spriteSettings.first, NULL, &rect);
        }

        void SpriteRenderComponent::Receive(engine::common::Event* message)
        {
            if (message->m_type == MessageType::COLLISION_HAPPENED)
            {
                engine::common::CollisionHappened* collisionHappened = message->GetMessage<engine::common::CollisionHappened>();
                if ((ComponentType::ProjectileCollision == collisionHappened->m_firstColComponentType)
                    && (ComponentType::ProjectileCollision != collisionHappened->m_secondColComponentType)
                    && (m_movement != MovementType::NO_MOVEMENT))
                {
                    if (m_movement == RIGHT)
                    {
                        m_movement = DESTROYED_LEFT;
                    }
                    else if (m_movement == LEFT)
                    {
                        m_movement = DESTROYED_RIGHT;
                    }
                }
            }
            else if (message->m_type == MessageType::DRIVE_CAR)
            {
                m_movement = CAR_DRIVE;
            }
        }

        ComponentType SpriteRenderComponent::GetComponentType()
        {
            return SpriteRender;
        }

        void SpriteRenderComponent::Destroy()
        {}
    }
}