#include "Event.hpp"

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