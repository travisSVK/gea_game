/**
 * @file     Event.hpp
 * @author   Marek Cernak
 * @date     3/24/2019
 *
 * @brief Events and EventPool declarations.
 */

#pragma once
#include "../managers/EntityManager.hpp"
#include "SDL.h"
#include "Enums.hpp"
#include "GLM/glm.hpp"
#include <vector>
#include <unordered_map>

namespace engine
{
    namespace common
    {
        /**
         * Base class for events.
         */
        class ENGINE_API Event
        {
        public:
            /**
            * Casts Event class to a type T - derived Event type.
            * @return pointer to the Event of type T.
            */
            template<typename T>
            T* GetMessage()
            {
                return static_cast<T*>(this);
            }
            managers::EntityManager::Entity m_entity;
            MessageType m_type;
        };

        /**
         * Pool of unused events.
         */
        class ENGINE_API EventPool
        {
        public:
            /**
            * Returns event of a specific type if any available.
            * type [in] Type of the event to retrieve from the pool.
            * @return pointer to the Event.
            */
            static Event* GetEvent(MessageType type);
            
            /**
            * Adds an event to the pool.
            * event [in/out] Event to be added to the pool.
            */
            static void AddEvent(Event* event);

            /**
            * Destroys all events in the pool.
            */
            static void DestroyEvents();

        private:
            static std::unordered_map<MessageType, std::vector<Event*>> m_eventPool;
        };

        class ENGINE_API Move : public Event
        {
        public:
            MovementType m_movement;
        };

        class ENGINE_API ChangePoisition : public Event
        {
        public:
            glm::dvec2 m_position;
            glm::dvec2 m_absolutePosition;
            MovementType m_movement;
        };

        class ENGINE_API CreateProjectile : public Event
        {
        public:
            char* m_projetileSprites;
            managers::EntityManager::Entity m_newEntity;
        };

        class ENGINE_API Crouch : public Event
        {
        };

        class ENGINE_API CollisionHappened : public Event
        {
        public:
            managers::EntityManager::Entity m_firstColEntity;
            managers::EntityManager::Entity m_secondColEntity;
            ComponentType m_firstColComponentType;
            ComponentType m_secondColComponentType;
            DirectionType m_firstColDirection;
            DirectionType m_secondColDirection;
        };

        class ENGINE_API MoveUp : public Event
        {
        public:
            std::vector<managers::EntityManager::Entity> m_excludedEntities;
            float m_moveLength;
        };

        class ENGINE_API ExcludeEntity : public Event
        {
        public:
            managers::EntityManager::Entity m_cameraEntity;
            managers::EntityManager::Entity m_excludedEntity;
        };

        class ENGINE_API IncludeEntity : public Event
        {
        public:
            managers::EntityManager::Entity m_cameraEntity;
            managers::EntityManager::Entity m_includedEntity;
        };

        class ENGINE_API MoveElevator : public Event
        {
        public:
            std::vector<managers::EntityManager::Entity> m_elevatorEntities;
            float m_moveLength;
        };

        class ENGINE_API PlayerEliminated : public Event
        {
        };

        class ENGINE_API PlayerFinished : public Event
        {
        };

        class ENGINE_API DriveCar : public Event
        {
        };

        class ENGINE_API OpenDoor : public Event
        {
        };

        class ENGINE_API PlayerHidden : public Event
        {
        public:
            managers::EntityManager::Entity m_playerEntity;
        };

        class ENGINE_API SpawnEnemy : public Event
        {
        public:
            glm::dvec2 m_position;
        };

        class ENGINE_API CreateEnemy : public Event
        {
        public:
            managers::EntityManager::Entity m_newEntity;
            glm::dvec2 m_position;
        };

        class ENGINE_API ExitGame : public Event
        {
        };
    }
}