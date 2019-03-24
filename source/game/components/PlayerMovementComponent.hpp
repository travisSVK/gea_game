#pragma once
#include "components/MovementComponent.hpp"

namespace game
{
    namespace components
    {
        class PlayerMovementComponent : public engine::components::MovementComponent
        {
        public:

            virtual void Create(engine::managers::EntityManager::Entity entity, const glm::dvec2& position, DirectionType direction);
            virtual std::vector<engine::common::Event*> Update() override;
            virtual void Destroy() override;
            virtual void Receive(engine::common::Event* message) override;
            virtual ComponentType GetComponentType() override;

        private:
            glm::dvec2 m_lastPosition;
            bool m_collisionUp;
            bool m_temporaryDisabled;
        };
    }
}