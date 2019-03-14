#pragma once
#include "components/MovementComponent.hpp"

class AIMovementComponent : public MovementComponent
{
public:

    virtual void Create(EntityManager::Entity entity, const glm::dvec2& position, const glm::dvec2& playerPosition, DirectionType direction, EntityManager* entityManager);
    virtual std::vector<Event*> Update() override;
    virtual void Destroy() override;
    virtual void Receive(Event* message) override;
    virtual ComponentType GetComponentType() override;

private:
    glm::dvec2 m_lastPosition;
    glm::dvec2 m_playerPosition;
    EntityManager* m_entityManager;
    float m_reloadTime;
    double m_leftBound;
    double m_rightBound;
};