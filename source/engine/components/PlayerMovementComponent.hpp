#pragma once
#include "MovementComponent.hpp"
#include "../EntityManager.hpp"

class ENGINE_API PlayerMovementComponent : public MovementComponent
{
public:

    PlayerMovementComponent(EntityManager::Entity entity, double positionX, double positionY, DirectionType direction);
    virtual bool Update(float dt, double& positionX, double& positionY) override;
    virtual void Receive(Message* message) override;

};