#pragma once
#include "../common/Enums.hpp"
#include "../EntityManager.hpp"
#include "../common/Event.hpp"
#include <unordered_map>
#include <functional>


template<typename System, typename Component>
class EventHandler
{
    typedef std::function<void(System*, const std::unordered_map<EntityManager::Entity, Component*>&, Event*)> Handler;
public:

    void RegisterHandler(MessageType messageType, const Handler& handler)
    {
        m_EventHandlers[messageType] = handler;
    }

    void HandleEvent(System* system, const std::unordered_map<EntityManager::Entity, Component*>& componentMap, Event* event)
    {
        m_EventHandlers[event->m_type](system, componentMap, event);
    }

private:
    std::unordered_map<MessageType, Handler> m_EventHandlers;
};