#pragma once
#include "../EngineAPI.hpp"
#include "MovementComponent.hpp"

class ENGINE_API ProjectileMovementComponent : public MovementComponent
{
public:
    ProjectileMovementComponent(EntityManager::Entity entity, double positionX, double positionY, DirectionType direction);
    virtual bool Update(float dt, double& positionX, double& positionY);
    virtual void Receive(Message* message);
};