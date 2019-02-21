#pragma once
#include "../EngineAPI.hpp"
#include "../EntityManager.hpp"
#include "RenderComponent.hpp"

struct SDL_Renderer;
struct SDL_Texture;
class ENGINE_API SpriteRenderComponent : public RenderComponent
{
public:
    SpriteRenderComponent(EntityManager::Entity entity, SDL_Renderer* renderer, const char* path, double positionX, double positionY);
    virtual void Update(float dt) override;
    virtual void Receive(Message* message) override;
    virtual RenderComponentType GetType() override;

protected:
    virtual void Draw();

private:
    SDL_Renderer* m_renderer;
    SDL_Texture* m_texture;
};