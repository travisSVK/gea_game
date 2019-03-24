#pragma once
#include "components/MovementComponent.hpp"

namespace game
{
    namespace components
    {
        class ElevatorMovementComponent : public engine::components::MovementComponent
        {
        public:
            virtual void Create(engine::managers::EntityManager::Entity entity, engine::managers::EntityManager::Entity playerEntity, const glm::dvec2& position, const glm::dvec2& playerPosition, const std::vector<engine::managers::EntityManager::Entity>& excludedEntities);
            virtual std::vector<engine::common::Event*> Update() override;
            virtual void Destroy() override;
            virtual void Receive(engine::common::Event* message);
            virtual ComponentType GetComponentType() override;

        protected:
            virtual bool IsPlayerWithinBounds();

        private:
            engine::managers::EntityManager::Entity m_playerEntity;
            glm::dvec2 m_playerPosition;
            bool m_move;
            std::vector<engine::managers::EntityManager::Entity> m_excludedEntities;
        };
    }
}