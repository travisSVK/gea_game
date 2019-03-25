/**
 * @file     MovementComponent.hpp
 * @author   Marek Cernak
 * @date     3/24/2019
 */

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
        /**
         * Movement component class used to update the position of the entity according to the movement.
         */
        class ENGINE_API MovementComponent
        {
        public:

            virtual ~MovementComponent() = default;
            
            /**
             * Creates the movement component with specified arguments.
             * entity [in] Entity the component is describing.
             * position [in] Entity position.
             * direction [in] Entity direction.
             */
            virtual void Create(managers::EntityManager::Entity entity, const glm::dvec2& position, DirectionType direction);
            
            /**
             * Updates the state. Can produce several Event.
             * @return Vector of events produced.
             */
            virtual std::vector<common::Event*> Update() = 0;

            /**
             * Destroys the component.
             */
            virtual void Destroy() = 0;
            
            /**
             * Process the received Event.
             * event [in] Event received to be processed.
             */
            virtual void Receive(common::Event* message) = 0;
            
            /**
             * @return Type of the component.
             */
            virtual ComponentType GetComponentType() = 0;
            
            /**
             * Sets the current position.
             * position [in] Poisition to update from. Position is the top left corner position of the entity.
             */
            void SetPosition(const glm::dvec2& position);

            /**
             * @return Entity that the component describes.
             */
            managers::EntityManager::Entity GetEntity();
            
            /**
             * @return Current position of the entity.
             */
            glm::dvec2 GetPosition();

            /**
             * @return Current direction of the entity.
             */
            DirectionType GetDirection();

            /**
             * @return Current movement type of the entity.
             */
            MovementType GetMovement();

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

            DirectionType m_direction;
            MovementType m_movement;
            glm::dvec2 m_position;
            managers::EntityManager::Entity m_entity;
            bool m_enabled;
        };
    }
}