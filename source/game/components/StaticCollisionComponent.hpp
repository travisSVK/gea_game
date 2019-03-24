#pragma once
#include "components/CollisionComponent.hpp"

namespace game
{
    namespace components
    {
        class StaticCollisionComponent : public engine::components::CollisionComponent
        {
        public:
            virtual void Create(engine::managers::EntityManager::Entity entity, const glm::dvec2& position, const glm::ivec2& size);
            virtual std::vector<engine::common::Event*> Update(const std::vector<CollisionComponent*>& collisionComponents) override;
            virtual void Destroy() override;
            virtual void Receive(engine::common::Event* message) override;
            virtual bool CheckCollision(CollisionComponent* collisionComponent, DirectionType& collisionDirection, DirectionType& collisionDirectionOther) override;
            virtual ComponentType GetComponentType() override;
        };
    }
}