#pragma once
#include "components/CollisionComponent.hpp"

class CharacterCollisionComponent : public CollisionComponent
{
public:
    virtual void Create(EntityManager::Entity entity, const glm::dvec2& position, const glm::ivec2& size, bool isPlayerEntity);
    virtual std::vector<Event*> Update(const std::vector<CollisionComponent*>& collisionComponents) override;
    virtual void Destroy() override;
    virtual void Receive(Event* message) override;
    virtual bool CheckCollision(CollisionComponent* collisionComponent, ComponentType collisionType, ComponentType& returnCollisionType, DirectionType& collisionDirection, DirectionType& collisionDirectionOther) override;
    virtual ComponentType GetComponentType() override;

private:
    bool m_isPlayerEntity;
};