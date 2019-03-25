/**
 * @file     RenderComponent.hpp
 * @author   Marek Cernak
 * @date     3/24/2019
 */

#pragma once
#include "../EngineAPI.hpp"
#include "../managers/EntityManager.hpp"
#include "../common/Enums.hpp"
#include "GLM/glm.hpp"

namespace engine
{
    namespace common
    {
        class Event;
    }
    namespace components
    {
        /**
         * Render component class used to update and draw the sprite of the entity.
         */
        class ENGINE_API RenderComponent
        {
        public:
            virtual ~RenderComponent() = default;

            /**
             * Creates the render component with specified arguments.
             * entity [in] Entity the component is describing.
             * position [in] Entity position.
             * movement [in] Entity movement type.
             */
            virtual void Create(managers::EntityManager::Entity entity, const glm::dvec2& position, MovementType movement);
            
            /**
             * Updates the state. Can produce Event.
             * @return pointer to Event when produced. Nullptr otherwise.
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
             * @return Current position of the sprite.
             */
            glm::dvec2 GetSpritePosition();

            /**
             * @return Current movement type of the entity.
             */
            MovementType GetMovementType();

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
            glm::dvec2 m_spritePosition;
            glm::dvec2 m_lastSpritePosition;
            managers::EntityManager::Entity m_entity;
            bool m_enabled;
            MovementType m_movement;
        };
    }
}