#include "DoorRenderComponent.hpp"
#include "common/Event.hpp"
#include "managers/SpritesheetManager.hpp"

void DoorRenderComponent::Create(EntityManager::Entity entity, EntityManager::Entity playerEntity, SDL_Renderer* renderer, const std::string& path, const glm::dvec2& position, const glm::dvec2& playerPosition)
{
    RenderComponent::Create(entity, position, NO_MOVEMENT);
    m_renderer = renderer;
    m_spriteIndex = 0;
    m_indexChangeRate = 0.0f;
    m_indexChangeRateTreshold = 0.1f;
    m_spritesheet = SpritesheetManager::ParseSpritesheet(path.c_str(), renderer);
    m_doorState = CLOSED;
    m_playerEntity = playerEntity;
    m_playerPosition = playerPosition;
}

Event* DoorRenderComponent::Update()
{
    Event* message = nullptr;

    if ((m_doorState == OPENING) || (m_doorState == CLOSING))
    {
        m_indexChangeRate += 0.016f;
        message = UpdateSpriteIndex();
    }

    Draw();
    return message;
}

Event* DoorRenderComponent::UpdateSpriteIndex()
{
    PlayerEliminated* message = nullptr; // TODO door closed message
    if (m_indexChangeRate >= m_indexChangeRateTreshold)
    {
        m_spriteIndex += 1;
        if (m_spriteIndex >= m_spritesheet[m_movement].size())
        {
            if (m_doorState == OPENING)
            {
                m_doorState = CLOSING;
                // TODO send player hidden message
                /*message = new PlayerEliminated();
                message->m_type = PLAYER_ELIMINATED;*/
            }
            else if (m_doorState == CLOSING)
            {
                m_doorState = CLOSED;
            }
            m_spriteIndex = 0;
        }
        m_indexChangeRate = 0.0f;
    }
    return message;
}

void DoorRenderComponent::Draw()
{
    std::pair<SDL_Texture*, glm::ivec2> spriteSettings = m_spritesheet[m_doorState][m_spriteIndex];
    SDL_Rect rect = { m_spritePosition.x, m_spritePosition.y, spriteSettings.second.x, spriteSettings.second.y };

    //Render texture to screen
    SDL_RenderCopy(m_renderer, spriteSettings.first, NULL, &rect);
}

void DoorRenderComponent::Receive(Event* message)
{
    if ((message->m_type == MessageType::OPEN_DOOR) && (m_doorState == CLOSED) && IsPlayerWithinBounds())
    {
        m_doorState = OPENING;
        m_indexChangeRate = 0.0f;
        m_spriteIndex = 0;
    }
    else if (message->m_type == MessageType::POSITION_CHANGE)
    {
        ChangePoisition* changePosition = message->GetMessage<ChangePoisition>();
        m_playerPosition = changePosition->m_absolutePosition;
    }
}

bool DoorRenderComponent::IsPlayerWithinBounds()
{
    if ((m_playerPosition.x >= m_spritePosition.x) && (m_playerPosition.x < (m_spritePosition.x + 30)) && (m_playerPosition.y == m_spritePosition.y + 20))
    {
        return true;
    }
    return false;
}

ComponentType DoorRenderComponent::GetComponentType()
{
    return DoorRender;
}

void DoorRenderComponent::Destroy()
{}