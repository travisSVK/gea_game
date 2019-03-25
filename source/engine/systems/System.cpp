/**
 * @file     System.cpp
 * @author   Marek Cernak
 * @date     3/25/2019
 */

#include "System.hpp"

namespace engine
{
    namespace systems
    {
        System::System() : m_enabled(true)
        {
            m_subject = new common::Subject();
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

        void System::Subscribe(const common::OnNotifyCallback& receiveCallback)
        {
            m_subject->addObserver(receiveCallback);
        }
        void System::Unsubscribe(const common::OnNotifyCallback& receiveCallback)
        {
            m_subject->removeObserver(receiveCallback);
        }

        void System::Notify(common::Event* event)
        {
            m_subject->notify(event);
        }
    }
}