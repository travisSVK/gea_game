#pragma once
#include "../managers/EntityManager.hpp"
#include <functional>
#include "../common/Enums.hpp"
#include "GLM/glm.hpp"
#include <vector>

class Event;
class ENGINE_API MovementComponent
{
public:

    virtual ~MovementComponent() = default;
    virtual std::vector<Event*> Update() = 0;
    virtual void Create(EntityManager::Entity entity, const glm::dvec2& position, DirectionType direction);
    virtual void Destroy() = 0;
    virtual void Receive(Event* message) = 0;
    virtual ComponentType GetComponentType() = 0;
    void SetPosition(const glm::dvec2& position);
    EntityManager::Entity GetEntity();
    glm::dvec2 GetPosition();
    DirectionType GetDirection();
    bool IsEnabled();
    void Enable();
    void Disable();

protected:
    
    DirectionType m_direction;
    MovementType m_movement;
    glm::dvec2 m_position;
    EntityManager::Entity m_entity;
    bool m_enabled;
};