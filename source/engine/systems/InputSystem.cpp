/**
 * @file     InputSystem.hpp
 * @author   Marek Cernak
 * @date     3/25/2019
 */

#include "InputSystem.hpp"
#include "../common/Event.hpp"
#include "../components/InputComponent.hpp"

namespace engine
{
    namespace systems
    {
        InputSystem::InputSystem(handlers::EventHandler<InputSystem, components::InputComponent>* eventHandler) : m_eventHandler(eventHandler), System()
        {
            m_components.reserve(5);
            m_componentLookUp.reserve(5);
        }

        void InputSystem::Update()
        {
            if (m_enabled)
            {
                SDL_Event event;
                while (SDL_PollEvent(&event))
                {
                    for (auto component : m_components)
                    {
                        if (component->IsEnabled())
                        {
                            common::Event* message = component->Update(event);
                            if (message)
                            {
                                message->m_entity = component->GetEntity();
                                Notify(message);
                                common::EventPool::AddEvent(message);
                            }
                        }
                    }
                }
            }
        }


        void InputSystem::Receive(common::Event* event)
        {
            if (m_enabled)
            {
                m_eventHandler->HandleEvent(this, m_componentLookUp, event);
            }
        }

        void InputSystem::DisableComponents(int excludeEntity)
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

        void InputSystem::Destroy()
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