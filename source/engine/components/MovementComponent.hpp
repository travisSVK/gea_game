#pragma once
#include "../EntityManager.hpp"
#include <functional>
#include "../common/Enums.hpp"

class Message;
class ENGINE_API MovementComponent
{
public:
    

    MovementComponent(EntityManager::Entity entity, double positionX, double positionY, DirectionType direction);
    virtual ~MovementComponent() {};
    virtual bool Update(float dt, double& positionX, double& positionY) = 0;
    virtual void Receive(Message* event) = 0;
    EntityManager::Entity GetEntity();
    double GetPositionX();
    double GetPositionY();
    DirectionType GetDirection();

protected:
    
    DirectionType m_direction;
    MovementType m_movement;
    double m_positionX;
    double m_positionY;
    EntityManager::Entity m_entity;
};