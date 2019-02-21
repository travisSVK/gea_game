#include "System.hpp"

System::System()
{
    m_subject = new Subject();
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