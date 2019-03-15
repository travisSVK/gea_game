#pragma once
#include "components/MovementComponent.hpp"

class PlayerMovementComponent : public MovementComponent
{
public:

    virtual void Create(EntityManager::Entity entity, const glm::dvec2& position, DirectionType direction);
    virtual std::vector<Event*> Update() override;
    virtual void Destroy() override;
    virtual void Receive(Event* message) override;
    virtual ComponentType GetComponentType() override;

private:
    glm::dvec2 m_lastPosition;
    bool m_collisionUp;
    bool m_temporaryDisabled;
};