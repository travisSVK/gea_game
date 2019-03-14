#pragma once
#include "components/MovementComponent.hpp"

class ProjectileMovementComponent : public MovementComponent
{
public:
    virtual void Create(EntityManager::Entity entity, const glm::dvec2& position, DirectionType direction);
    virtual std::vector<Event*> Update();
    virtual void Destroy() override;
    virtual void Receive(Event* message);
    virtual ComponentType GetComponentType() override;
};