/**
 * @file     CollisionComponent.hpp
 * @author   Marek Cernak
 * @date     3/24/2019
 */

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
        /**
         * Collision component class used to update the collision state.
         */
        class ENGINE_API CollisionComponent
        {
        public:
            virtual ~CollisionComponent() = default;
            
            /**
             * Creates the collision component with specified arguments.
             * entity [in] Entity the component is describing.
             * position [in] Position of the entity the collision component describes.
             * size [in] Size of the collision 2D bounding box.
             */
            virtual void Create(managers::EntityManager::Entity entity, const glm::dvec2& position, const glm::ivec2& size);
            
            /**
             * Updates the state. Can produce several events - depending on the number of collisions.
             * collisionComponents [in] Collision components to check the collision with.
             * @return Vector of produed events.
             */
            virtual std::vector<common::Event*> Update(const std::vector<CollisionComponent*>& collisionComponents) = 0;
            
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
             * Sets the current collision bounding box position.
             * position [in] Poisition to update from. Position is the top left corner position of the bounding box.
             */
            void SetPosition(const glm::dvec2& position);

            /**
             * Sets the current collision bounding box position.
             * position [in] Poisition to update from. Position is the center position of the bounding box.
             */
            void SetCenterPosition(const glm::dvec2& position);

            /**
             * Checks the collision with other collision component.
             * collisionComponent [in] Collision component to check the collision with.
             * collisionDirection [out] Collision direction to be set (if collision detected) for the collsion component.
             * collisionDirection [out] Collision direction to be set (if collision detected) for the other collsion component.
             * @return True if collision happened, False otherwise
             */
            virtual bool CheckCollision(CollisionComponent* collisionComponent, DirectionType& collisionDirection, DirectionType& collisionDirectionOther);

            /**
             * @return Entity that the component describes.
             */
            managers::EntityManager::Entity GetEntity();
            
            /**
             * @return Current position of the center of the bounding box.
             */
            glm::dvec2 GetPosition();

            /**
             * @return Size of the bounding box.
             */
            glm::ivec2 GetSize();

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
            glm::dvec2 m_position;
            glm::dvec2 m_lastPosition;
            glm::ivec2 m_size;
            managers::EntityManager::Entity m_entity;
            bool m_enabled;
        };
    }
}