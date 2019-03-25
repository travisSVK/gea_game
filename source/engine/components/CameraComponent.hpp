/**
 * @file     CameraComponent.hpp
 * @author   Marek Cernak
 * @date     3/24/2019
 */

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
        /**
         * Camera component class used to update the camera state.
         */
        class ENGINE_API CameraComponent
        {
        public:
            virtual ~CameraComponent() = default;
            
            /**
             * Creates the camera component with specified arguments.
             * entity [in] Entity the component is describing.
             * position [in] Camera position.
             * excludedEntities [in] List of entities to exclude from camera movement.
             */
            virtual void Create(managers::EntityManager::Entity entity, const glm::dvec2& position, const std::vector<managers::EntityManager::Entity>& excludedEntities);
            
            /**
             * Updates the state. Can produce new event.
             * @return new Event or nullptr if no Event produced.
             */
            virtual common::Event* Update() = 0;

            /**
             * Destroys the component.
             */
            virtual void Destroy() = 0;
            
            /**
             * Process the received Event.
             * event [in] Event received to be processed.
             */
            virtual void Receive(common::Event* event);

            /**
             * @return Type of the component.
             */
            virtual ComponentType GetComponentType() = 0;

            /**
             * Sets current camera position.
             * position [in] Poisition to update from.
             */
            void SetPosition(const glm::dvec2& position);
            
            /**
             * @return Entity that the component describes.
             */
            managers::EntityManager::Entity GetEntity();
            
            /**
             * @return True if the component is enabled, False otherwise.
             */
            bool IsEnabled();

            /**
             * Enables the component.
             */
            void Enable();

            /**
             * Disables the component.
             */
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