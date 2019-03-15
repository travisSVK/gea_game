#include "EnemyDoorRenderComponent.hpp"
#include "common/Event.hpp"
#include "managers/SpritesheetManager.hpp"
#include "../RandomNumberGenerator.hpp"

void EnemyDoorRenderComponent::Create(EntityManager::Entity entity, SDL_Renderer* renderer, const std::string& path, const glm::dvec2& position, EntityManager* entityManager)
{
    RenderComponent::Create(entity, position, NO_MOVEMENT);
    m_renderer = renderer;
    m_spriteIndex = 0;
    m_indexChangeRate = 0.0f;
    m_indexChangeRateTreshold = 0.1f;
    if (path.compare(m_path) != 0)
    {
        m_path = path;
        m_spritesheet = SpritesheetManager::ParseSpritesheet(path.c_str(), renderer);
    }
    m_doorState = CLOSED;
    m_entityManager = entityManager;
}

Event* EnemyDoorRenderComponent::Update()
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

Event* EnemyDoorRenderComponent::UpdateSpriteIndex()
{
    CreateEnemy* message = nullptr;
    if (m_indexChangeRate >= m_indexChangeRateTreshold)
    {
        m_spriteIndex += 1;
        if (m_spriteIndex >= m_spritesheet[m_doorState].size())
        {
            if (m_doorState == OPENING)
            {
                m_doorState = CLOSING;
                Event* event = EventPool::GetEvent(CREATE_ENEMY);
                if (event)
                {
                    message = event->GetMessage<CreateEnemy>();
                }
                else
                {
                    message = new CreateEnemy();
                }
                message->m_type = CREATE_ENEMY;
                message->m_position = glm::dvec2(m_spritePosition.x, m_spritePosition.y + 10);
                message->m_newEntity = m_entityManager->CreateEntity();
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

void EnemyDoorRenderComponent::Draw()
{
    std::pair<SDL_Texture*, glm::ivec2> spriteSettings = m_spritesheet[m_doorState][m_spriteIndex];
    SDL_Rect rect = { m_spritePosition.x, m_spritePosition.y, spriteSettings.second.x, spriteSettings.second.y };

    //Render texture to screen
    SDL_RenderCopy(m_renderer, spriteSettings.first, NULL, &rect);
}

void EnemyDoorRenderComponent::Receive(Event* message)
{
    if ((message->m_type == MessageType::SPAWN_ENEMY) && (m_doorState == CLOSED))
    {
        SpawnEnemy* spawnEnemy = message->GetMessage<SpawnEnemy>();
        if (spawnEnemy->m_position.y == m_spritePosition.y)
        {
            // open randomly
            uint16_t random = RandomNumberGenerator::GenerateRandomNumber(0, 10);
            if (random >= 5)
            {
                m_doorState = OPENING;
                m_indexChangeRate = 0.0f;
                m_spriteIndex = 0;
            }
        }
    }
}

ComponentType EnemyDoorRenderComponent::GetComponentType()
{
    return EnemyDoorRender;
}

void EnemyDoorRenderComponent::Destroy()
{}