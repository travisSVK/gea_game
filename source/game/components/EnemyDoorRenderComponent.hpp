#pragma once
#include "components/RenderComponent.hpp"
#include <unordered_map>

struct SDL_Renderer;
struct SDL_Texture;
class EnemyDoorRenderComponent : public RenderComponent
{
public:

    virtual void Create(EntityManager::Entity entity, SDL_Renderer* renderer, const std::string& path, const glm::dvec2& position, EntityManager* entityManager);
    virtual Event* Update() override;
    virtual void Destroy() override;
    virtual void Receive(Event* message) override;
    virtual ComponentType GetComponentType() override;

protected:
    virtual void Draw();
    virtual Event* UpdateSpriteIndex();

private:
    std::unordered_map<unsigned int, std::vector<std::pair<SDL_Texture*, glm::ivec2>>> m_spritesheet;
    SDL_Renderer* m_renderer;
    unsigned int m_spriteIndex;
    float m_indexChangeRate;
    float m_indexChangeRateTreshold;
    DoorState m_doorState;
    EntityManager* m_entityManager;
    std::string m_path;
};