#pragma once
#include "../EngineAPI.hpp"
#include "../managers/EntityManager.hpp"
#include "../common/Enums.hpp"
#include <vector>
#include "GLM/glm.hpp"

namespace engine
{
    namespace common
    {
        class Event;
    }
    namespace components
    {
        class ENGINE_API CollisionComponent
        {
        public:
            CollisionComponent() = default;
            virtual ~CollisionComponent() = default;
            virtual void Create(managers::EntityManager::Entity entity, const glm::dvec2& position, const glm::ivec2& size);
            virtual std::vector<common::Event*> Update(const std::vector<CollisionComponent*>& collisionComponents) = 0;
            virtual void Destroy() = 0;
            virtual void Receive(common::Event* message) = 0;
            virtual ComponentType GetComponentType() = 0;
            void SetPosition(const glm::dvec2& position);
            void SetCenterPosition(const glm::dvec2& position);
            virtual bool CheckCollision(CollisionComponent* collisionComponent, DirectionType& collisionDirection, DirectionType& collisionDirectionOther);

            managers::EntityManager::Entity GetEntity();
            glm::dvec2 GetPosition();
            glm::ivec2 GetSize();
            bool IsEnabled();
            void Enable();
            void Disable();

        protected:
            glm::dvec2 m_position;
            glm::dvec2 m_lastPosition;
            glm::ivec2 m_size;
            managers::EntityManager::Entity m_entity;
            bool m_enabled;
        };
    }
}