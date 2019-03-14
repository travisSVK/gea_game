#include "StaticRenderComponent.hpp"
#include "common/Event.hpp"
#include "managers/RenderManager.hpp"

void StaticRenderComponent::Create(EntityManager::Entity entity, SDL_Renderer* renderer, const std::string& path, const glm::dvec2& position, const glm::ivec2& size)
{
    RenderComponent::Create(entity, position, MovementType::NO_MOVEMENT);
    m_renderer = renderer;
    m_size = size;
    m_texture = RenderManager::CreateTexture(renderer, path.c_str());
}

Event* StaticRenderComponent::Update()
{
    Draw();
    return nullptr;
}

void StaticRenderComponent::Draw()
{
    SDL_Rect rect = { m_spritePosition.x, m_spritePosition.y, m_size.x, m_size.y };

    //Render texture to screen
    SDL_RenderCopy(m_renderer, m_texture, NULL, &rect);
}

void StaticRenderComponent::Receive(Event* message)
{
}

ComponentType StaticRenderComponent::GetComponentType()
{
    return StaticRender;
}

void StaticRenderComponent::Destroy()
{
}