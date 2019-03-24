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
        template<typename System, typename Component>
        class EventHandler
        {
            typedef std::function<void(System*, const std::unordered_map<managers::EntityManager::Entity, Component*>&, common::Event*)> Handler;
        public:

            void RegisterHandler(MessageType messageType, const Handler& handler)
            {
                m_EventHandlers[messageType] = handler;
            }

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