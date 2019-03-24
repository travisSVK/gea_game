#pragma once
#include "components/MovementComponent.hpp"

namespace game
{
    namespace components
    {
        class AIMovementComponent : public engine::components::MovementComponent
        {
        public:

            virtual void Create(engine::managers::EntityManager::Entity entity, const glm::dvec2& position, const glm::dvec2& playerPosition, DirectionType direction, engine::managers::EntityManager* entityManager);
            virtual std::vector<engine::common::Event*> Update() override;
            virtual void Destroy() override;
            virtual void Receive(engine::common::Event* message) override;
            virtual ComponentType GetComponentType() override;

        private:
            glm::dvec2 m_lastPosition;
            glm::dvec2 m_playerPosition;
            engine::managers::EntityManager* m_entityManager;
            float m_reloadTime;
            double m_leftBound;
            double m_rightBound;
        };
    }
}