#pragma once
#include "../EngineAPI.hpp"
#include "../managers/EntityManager.hpp"
#include "GLM/glm.hpp"
#include <vector>
#include "../common/Enums.hpp"

class Event;
class ENGINE_API CameraComponent
{
public:
    CameraComponent() = default;
    virtual ~CameraComponent() = default;
    virtual void Create(EntityManager::Entity entity, const glm::dvec2& position, const std::vector<EntityManager::Entity>& excludedEntities);
    virtual Event* Update() = 0;
    virtual void Destroy() = 0;
    virtual void Receive(Event* message);
    virtual ComponentType GetComponentType() = 0;
    void SetPosition(glm::dvec2 position);
    EntityManager::Entity GetEntity();
    bool IsEnabled();
    void Enable();
    void Disable();

protected:

    glm::dvec2 m_positionChange;
    glm::dvec2 m_position;
    std::vector<EntityManager::Entity> m_excludedEntities;
    bool m_enabled;

private:
    
    EntityManager::Entity m_entity;
};