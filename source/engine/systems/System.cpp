#include "System.hpp"

System::System() : m_enabled(true)
{
    m_subject = new Subject();
}

void System::Disable(int excludeEntity)
{
    m_enabled = false;
    DisableComponents(excludeEntity);
}

void System::Enable()
{
    m_enabled = true;
}

void System::Subscribe(const OnNotifyCallback& receiveCallback)
{
    m_subject->addObserver(receiveCallback);
}
void System::Unsubscribe(const OnNotifyCallback& receiveCallback)
{
    m_subject->removeObserver(receiveCallback);
}

void System::Notify(Event* event)
{
    m_subject->notify(event);
}