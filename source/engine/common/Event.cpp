/**
 * @file     Event.cpp
 * @author   Marek Cernak
 * @date     3/24/2019
 *
 * @brief Events and EventPool definitions.
 */
#include "Event.hpp"

namespace engine
{
    namespace common
    {
        std::unordered_map<MessageType, std::vector<Event*>> EventPool::m_eventPool;
        Event* EventPool::GetEvent(MessageType type)
        {
            if ((m_eventPool.find(type) != m_eventPool.end()) && (m_eventPool[type].size() != 0))
            {
                Event* event = m_eventPool[type].back();
                m_eventPool[type].pop_back();
                return event;
            }
            return nullptr;
        }

        void EventPool::AddEvent(Event* event)
        {
            m_eventPool[event->m_type].push_back(event);
        }

        void EventPool::DestroyEvents()
        {
            for (const auto& event : m_eventPool)
            {
                for (auto e : event.second)
                {
                    delete e;
                }
            }
        }
    }
}