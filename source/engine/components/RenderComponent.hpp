#pragma once
#include "../EngineAPI.hpp"
#include "../managers/EntityManager.hpp"
#include "../common/Enums.hpp"
#include "GLM/glm.hpp"

class Event;
class ENGINE_API RenderComponent
{
public:
    RenderComponent() = default;
    virtual void Create(EntityManager::Entity entity, const glm::dvec2& position, MovementType movement);
    virtual ~RenderComponent() = default;
    virtual Event* Update() = 0;
    virtual void Destroy() = 0;
    virtual void Receive(Event* message) = 0;
    void SetPosition(const glm::dvec2& position);
    EntityManager::Entity GetEntity();
    glm::dvec2 GetSpritePosition();
    MovementType GetMovementType();
    bool IsEnabled();
    void Enable();
    void Disable();
    virtual ComponentType GetComponentType() = 0;

protected:
    glm::dvec2 m_spritePosition;
    glm::dvec2 m_lastSpritePosition;
    EntityManager::Entity m_entity;
    bool m_enabled;
    MovementType m_movement;
};