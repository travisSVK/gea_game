#pragma once
#include "../EngineAPI.hpp"
#include "../managers/EntityManager.hpp"
#include "GLM/glm.hpp"
#include <vector>
#include "../common/Enums.hpp"

namespace engine
{
    namespace common
    {
        class Event;
    }
    namespace components
    {
        class ENGINE_API CameraComponent
        {
        public:
            CameraComponent() = default;
            virtual ~CameraComponent() = default;
            virtual void Create(managers::EntityManager::Entity entity, const glm::dvec2& position, const std::vector<managers::EntityManager::Entity>& excludedEntities);
            virtual common::Event* Update() = 0;
            virtual void Destroy() = 0;
            virtual void Receive(common::Event* message);
            virtual ComponentType GetComponentType() = 0;
            void SetPosition(glm::dvec2 position);
            managers::EntityManager::Entity GetEntity();
            bool IsEnabled();
            void Enable();
            void Disable();

        protected:

            glm::dvec2 m_positionChange;
            glm::dvec2 m_position;
            std::vector<managers::EntityManager::Entity> m_excludedEntities;
            bool m_enabled;

        private:

            managers::EntityManager::Entity m_entity;
        };
    }
}