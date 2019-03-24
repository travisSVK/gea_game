#pragma once
#include "components/CollisionComponent.hpp"

namespace game
{
    namespace components
    {
        class ProjectileCollisionComponent : public engine::components::CollisionComponent
        {
        public:
            virtual void Create(engine::managers::EntityManager::Entity entity,
                engine::managers::EntityManager::Entity spawnedFromEntity,
                int targetEntity,
                const glm::dvec2& position, const glm::ivec2& size);
            virtual std::vector<engine::common::Event*> Update(const std::vector<CollisionComponent*>& collisionComponents) override;
            virtual void Destroy() override;
            virtual void Receive(engine::common::Event* message) override;
            virtual bool CheckCollision(CollisionComponent* collisionComponent, DirectionType& collisionDirection, DirectionType& collisionDirectionOther) override;
            virtual ComponentType GetComponentType() override;

        private:
            engine::managers::EntityManager::Entity m_spawnedFromEntity;
            int m_targetEntity;
        };
    }
}