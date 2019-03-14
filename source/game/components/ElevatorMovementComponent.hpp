#pragma once
#include "components/MovementComponent.hpp"

class ElevatorMovementComponent : public MovementComponent
{
public:
    virtual void Create(EntityManager::Entity entity, EntityManager::Entity playerEntity, const glm::dvec2& position, const glm::dvec2& playerPosition, const std::vector<EntityManager::Entity>& excludedEntities);
    virtual std::vector<Event*> Update() override;
    virtual void Destroy() override;
    virtual void Receive(Event* message);
    virtual ComponentType GetComponentType() override;

protected:
    virtual bool IsPlayerWithinBounds();

private:
    EntityManager::Entity m_playerEntity;
    glm::dvec2 m_playerPosition;
    bool m_move;
    std::vector<EntityManager::Entity> m_excludedEntities;
};