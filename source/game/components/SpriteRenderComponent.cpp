#include "SpriteRenderComponent.hpp"
#include "common/Event.hpp"
#include "managers/SpritesheetManager.hpp"

void SpriteRenderComponent::Create(EntityManager::Entity entity, SDL_Renderer* renderer, const std::string& path, const glm::dvec2& position, MovementType movementType)
{
    RenderComponent::Create(entity, position, movementType);
    m_renderer = renderer;
    m_destroyedChangeRate = 0.0f;
    m_destroyedChangeRateTreshold = 0.0f;
    if (path.compare(m_path) != 0)
    {
        m_path = path;
        m_sprites = SpritesheetManager::ParseSprites(path.c_str(), renderer);
    }
}

Event* SpriteRenderComponent::Update()
{
    Event* message = nullptr;
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
            DriveCar* driveCar;
            Event* event = EventPool::GetEvent(DRIVE_CAR);
            if (event)
            {
                driveCar = event->GetMessage<DriveCar>();
            }
            else
            {
                driveCar = new DriveCar();
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

void SpriteRenderComponent::Receive(Event* message)
{
    if (message->m_type == MessageType::COLLISION_HAPPENED)
    {
        CollisionHappened* collisionHappened = message->GetMessage<CollisionHappened>();
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