/**
 * @file     EventHandler.hpp
 * @author   Marek Cernak
 * @date     3/24/2019
 */

#pragma once
#include "../common/Enums.hpp"
#include "../managers/EntityManager.hpp"
#include "../common/Event.hpp"
#include <unordered_map>
#include <functional>

namespace engine
{
    namespace handlers
    {
        /**
         * Event handler class used to handle the events received by the systems (observers).
         */
        template<typename System, typename Component>
        class EventHandler
        {
            typedef std::function<void(System*, const std::unordered_map<managers::EntityManager::Entity, Component*>&, common::Event*)> Handler;
        public:

            /**
             * Registers new handler for the specific message type.
             * messageType [in] Type of the message to be handled by the handler.
             * handler [in] Handler function to be called when the message of messageType is received.
             */
            void RegisterHandler(MessageType messageType, const Handler& handler)
            {
                m_EventHandlers[messageType] = handler;
            }

            /**
             * Handles the event by calling the handler for the message type.
             * system [in] System that received the event.
             * componentMap [in] Map of components that the system contains.
             * event [in] Event to be handled.
             */
            void HandleEvent(System* system, const std::unordered_map<managers::EntityManager::Entity, Component*>& componentMap, common::Event* event)
            {
                if (m_EventHandlers.find(event->m_type) != m_EventHandlers.end())
                {
                    m_EventHandlers[event->m_type](system, componentMap, event);
                }
            }

        private:
            std::unordered_map<MessageType, Handler> m_EventHandlers;
        };
    }
}