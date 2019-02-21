#pragma once
#include "../EngineAPI.hpp"
#include "../EntityManager.hpp"
#include "../common/Enums.hpp"

class Message;
class ENGINE_API RenderComponent
{
public:
    RenderComponent(EntityManager::Entity entity, double positionX, double positionY);
    virtual ~RenderComponent() = default;
    virtual void Update(float dt) = 0;
    virtual RenderComponentType GetType() = 0;
    virtual void Receive(Message* message) = 0;
    virtual void SetPosition(double positionX, double positionY);
    EntityManager::Entity GetEntity();
    double GetSpritePositionX();
    double GetSpritePositionY();

protected:
    double m_spritePositionX;
    double m_spritePositionY;
    EntityManager::Entity m_entity;
    // TODO add component types and then I can register component based on type and its constructor
};