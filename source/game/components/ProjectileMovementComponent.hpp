#pragma once
#include "components/MovementComponent.hpp"

namespace game
{
    namespace components
    {
        class ProjectileMovementComponent : public engine::components::MovementComponent
        {
        public:
            virtual void Create(engine::managers::EntityManager::Entity entity, const glm::dvec2& position, DirectionType direction);
            virtual std::vector<engine::common::Event*> Update();
            virtual void Destroy() override;
            virtual void Receive(engine::common::Event* message);
            virtual ComponentType GetComponentType() override;
        };
    }
}