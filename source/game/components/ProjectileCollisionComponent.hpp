#pragma once
#include "components/CollisionComponent.hpp"

class ProjectileCollisionComponent : public CollisionComponent
{
public:
    virtual void Create(EntityManager::Entity entity,
        EntityManager::Entity spawnedFromEntity,
        int targetEntity,
        const glm::dvec2& position, const glm::ivec2& size);
    virtual std::vector<Event*> Update(const std::vector<CollisionComponent*>& collisionComponents) override;
    virtual void Destroy() override;
    virtual void Receive(Event* message) override;
    virtual bool CheckCollision(CollisionComponent* collisionComponent, DirectionType& collisionDirection, DirectionType& collisionDirectionOther) override;
    virtual ComponentType GetComponentType() override;

private:
    EntityManager::Entity m_spawnedFromEntity;
    int m_targetEntity;
};