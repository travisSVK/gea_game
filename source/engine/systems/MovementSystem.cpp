#include "MovementSystem.hpp"
#include "../common/Event.hpp"
#include "../components/MovementComponent.hpp"

namespace engine
{
    namespace systems
    {
        MovementSystem::MovementSystem(handlers::EventHandler<MovementSystem, components::MovementComponent>* eventHandler) : m_eventHandler(eventHandler), System()
        {
            m_components.reserve(100);
            m_componentLookUp.reserve(100);
        }

        void MovementSystem::Update()
        {
            if (m_enabled)
            {
                for (auto component : m_components)
                {
                    if (component->IsEnabled())
                    {
                        std::vector<common::Event*> events = component->Update();
                        for (auto event : events)
                        {
                            event->m_entity = component->GetEntity();
                            Notify(event);
                            common::EventPool::AddEvent(event);
                        }
                    }
                }
            }
        }


        void MovementSystem::Receive(common::Event* event)
        {
            if (m_enabled)
            {
                m_eventHandler->HandleEvent(this, m_componentLookUp, event);
            }
        }

        void MovementSystem::DisableComponents(int excludeEntity)
        {
            for (auto component : m_components)
            {
                if ((component->GetEntity() != excludeEntity) && (component->IsEnabled()))
                {
                    component->Disable();                }
                else
                {
                    m_enabled = true;
                }
            }
        }

        void MovementSystem::Destroy()
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