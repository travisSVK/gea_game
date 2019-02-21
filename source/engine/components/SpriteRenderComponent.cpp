#include "SpriteRenderComponent.hpp"
#include "../common/Event.hpp"
#include "../managers/RenderManager.hpp"

SpriteRenderComponent::SpriteRenderComponent(EntityManager::Entity entity, SDL_Renderer* renderer, const char* path, double positionX, double positionY) :
    RenderComponent(entity, positionX, positionY), m_renderer(renderer)
{
    m_texture = RenderManager::CreateTexture(renderer, path);
}

void SpriteRenderComponent::Update(float dt)
{
    Draw();
}

void SpriteRenderComponent::Draw()
{
    SDL_Rect rect = { m_spritePositionX, m_spritePositionY, 20, 5 };

    //Render texture to screen
    SDL_RenderCopy(m_renderer, m_texture, NULL, &rect);
}

void SpriteRenderComponent::Receive(Message* message)
{
    // nothing
}

RenderComponentType SpriteRenderComponent::GetType()
{
    return RenderComponentType::Sprite;
}