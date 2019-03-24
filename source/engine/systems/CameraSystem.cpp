#include "CameraSystem.hpp"
#include "../common/Event.hpp"
#include "../components/CameraComponent.hpp"

namespace engine
{
    namespace systems
    {
        CameraSystem::CameraSystem(handlers::EventHandler<CameraSystem, components::CameraComponent>* eventHandler) : m_eventHandler(eventHandler)
        {
            m_components.reserve(2);
            m_componentLookUp.reserve(2);
        }

        void CameraSystem::Update()
        {
            if (m_enabled)
            {
                for (auto component : m_components)
                {
                    if (component->IsEnabled())
                    {
                        common::Event* event = component->Update();
                        if (event)
                        {
                            event->m_entity = component->GetEntity();
                            Notify(event);
                            common::EventPool::AddEvent(event);
                        }
                    }
                }
            }
        }

        void CameraSystem::Receive(common::Event* event)
        {
            if (m_enabled)
            {
                m_eventHandler->HandleEvent(this, m_componentLookUp, event);
            }
        }

        void CameraSystem::DisableComponents(int excludeEntity)
        {
            for (auto component : m_components)
            {
                if ((component->GetEntity() != excludeEntity) && (component->IsEnabled()))
                {
                    component->Disable();
                }
                else
                {
                    m_enabled = true;
                }
            }
        }

        void CameraSystem::Destroy()
        {
            for (auto component : m_components)
            {
                component->Destroy();
                delete component;
            }
            for (const auto& constructor : m_baseConstructors)
            {
                delete constructor.second;
            }
            delete m_eventHandler;
        }
    }
}