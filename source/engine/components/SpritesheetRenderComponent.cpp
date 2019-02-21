#include "SpritesheetRenderComponent.hpp"
#include "../common/Event.hpp"
#include <filesystem>
#include "../managers/RenderManager.hpp"

SpritesheetRenderComponent::SpritesheetRenderComponent(
    EntityManager::Entity entity, SDL_Renderer* renderer, const char* path, double positionX, double positionY):
    RenderComponent(entity, positionX, positionY), m_renderer(renderer), m_movement(NO_MOVEMENT_RIGHT), m_spriteIndex(0), m_indexChangeRate(0.0f)
{
    // create spritesheet here
    SDL_Texture* texture;
    std::string imagesPath(path);
    imagesPath += "/left_move/";
    std::vector<SDL_Texture*> textures;
    textures.reserve(5);
    for (const auto& entry : std::experimental::filesystem::directory_iterator(imagesPath))
    {
        texture = RenderManager::CreateTexture(renderer, entry.path().generic_string().c_str());
        if (texture)
        {
            textures.push_back(texture);
        }
    }
    m_spritesheet[MovementType::LEFT] = textures;
            
    textures.clear();
    imagesPath = path;
    imagesPath += "/right_move/";
    for (const auto& entry : std::experimental::filesystem::directory_iterator(imagesPath))
    {
        texture = RenderManager::CreateTexture(renderer, entry.path().generic_string().c_str());
        if (texture)
        {
            textures.push_back(texture);
        }
    }
    m_spritesheet[MovementType::RIGHT] = textures;
    
    textures.clear();
    textures.reserve(1);
    imagesPath = path;
    imagesPath += "/right_stand.png";
    texture = RenderManager::CreateTexture(renderer, imagesPath.c_str());
    if (texture)
    {
        textures.push_back(texture);
        m_spritesheet[MovementType::NO_MOVEMENT_RIGHT] = textures;
    }
    
    textures.clear();
    imagesPath = path;
    imagesPath += "/left_stand.png";
    texture = RenderManager::CreateTexture(renderer, imagesPath.c_str());
    if (texture)
    {
        textures.push_back(texture);
        m_spritesheet[MovementType::NO_MOVEMENT_LEFT] = textures;
    }
}

void SpritesheetRenderComponent::Update(float dt)
{
    if ((m_movement == LEFT) || (m_movement == RIGHT))
    {
        m_indexChangeRate += dt;
        UpdateSpriteIndex();
    }
    Draw();
}

void SpritesheetRenderComponent::UpdateSpriteIndex()
{
    if (m_indexChangeRate > 0.1f)
    {
        m_spriteIndex += 1;
        if (m_spriteIndex >= m_spritesheet[m_movement].size())
        {
            m_spriteIndex = 0;
        }
        m_indexChangeRate = 0.0f;
    }
}

void SpritesheetRenderComponent::Draw()
{
    SDL_Rect rect = { m_spritePositionX, m_spritePositionY, 50, 80 };

    //Render texture to screen
    SDL_RenderCopy(m_renderer, m_spritesheet[m_movement][m_spriteIndex], NULL, &rect);
}

void SpritesheetRenderComponent::Receive(Message* message)
{
    if (message->m_type == MessageType::MOVE)
    {
        Move* move = message->GetMessage<Move>();
        switch (move->m_movement)
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
        case MovementType::NO_MOVEMENT:
            if (m_movement == LEFT)
            {
                m_movement = NO_MOVEMENT_LEFT;
            }
            else if (m_movement == RIGHT)
            {
                m_movement = NO_MOVEMENT_RIGHT;
            }
            m_spriteIndex = 0;
            break;

        default: break;
        }
    }
}

RenderComponentType SpritesheetRenderComponent::GetType()
{
    return RenderComponentType::Spritesheet;
}