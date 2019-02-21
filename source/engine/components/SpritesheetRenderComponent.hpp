#pragma once
#include "../EngineAPI.hpp"
#include "RenderComponent.hpp"
#include <unordered_map>

struct SDL_Renderer;
struct SDL_Texture;
class ENGINE_API SpritesheetRenderComponent : public RenderComponent
{
public:
    
    SpritesheetRenderComponent(EntityManager::Entity entity, SDL_Renderer* renderer, const char* path, double positionX, double positionY);
    virtual void Update(float dt) override;
    virtual void Receive(Message* Message) override;
    virtual RenderComponentType GetType() override;

protected:
    virtual void Draw();
    virtual void UpdateSpriteIndex();

private:
    std::unordered_map<MovementType, std::vector<SDL_Texture*>> m_spritesheet;
    SDL_Renderer* m_renderer;
    MovementType m_movement;
    unsigned int m_spriteIndex;
    float m_indexChangeRate;
};