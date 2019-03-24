#pragma once
#include "../managers/EntityManager.hpp"
#include <functional>
#include "../common/Enums.hpp"
#include "GLM/glm.hpp"
#include <vector>

namespace engine
{
    namespace common
    {
        class Event;
    }
    namespace components
    {
        class ENGINE_API MovementComponent
        {
        public:

            virtual ~MovementComponent() = default;
            virtual std::vector<common::Event*> Update() = 0;
            virtual void Create(managers::EntityManager::Entity entity, const glm::dvec2& position, DirectionType direction);
            virtual void Destroy() = 0;
            virtual void Receive(common::Event* message) = 0;
            virtual ComponentType GetComponentType() = 0;
            void SetPosition(const glm::dvec2& position);
            managers::EntityManager::Entity GetEntity();
            glm::dvec2 GetPosition();
            DirectionType GetDirection();
            MovementType GetMovement();
            bool IsEnabled();
            void Enable();
            void Disable();

        protected:

            DirectionType m_direction;
            MovementType m_movement;
            glm::dvec2 m_position;
            managers::EntityManager::Entity m_entity;
            bool m_enabled;
        };
    }
}