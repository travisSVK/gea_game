#include "InputSystem.hpp"
#include "../common/Event.hpp"
#include "../components/InputComponent.hpp"
#include <chrono>
#include <iostream>

InputSystem::InputSystem(EventHandler<InputSystem, InputComponent>* eventHandler) : m_eventHandler(eventHandler), System()
{
    m_components.reserve(5);
    m_componentLookUp.reserve(5);
    m_counter = 0;
    m_elapsed = 0.0;
}

void InputSystem::Update()
{
    /*m_counter++;
    auto start = std::chrono::high_resolution_clock::now();*/
    if (m_enabled)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            for (auto component : m_components)
            {
                if (component->IsEnabled())
                {
                    Event* message = component->Update(event);
                    if (message)
                    {
                        message->m_entity = component->GetEntity();
                        Notify(message);
                        EventPool::AddEvent(message);
                        //delete event;
                    }
                }
                /*if (!component->IsEnabled())
                {
                    bool isDisabled = false;
                    for (auto disabledComponent : m_disabledComponents[component->GetComponentType()])
                    {
                        if (disabledComponent->GetEntity() == component->GetEntity())
                        {
                            isDisabled = true;
                            break;
                        }
                    }
                    if (!isDisabled)
                    {
                        m_disabledComponents[component->GetComponentType()].push_back(component);
                    }
                }*/
            }
        }
    }
    /*auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    m_elapsed += elapsed.count();
    if (m_counter == 100)
    {
        std::cout << "InputSystem: " << m_elapsed << std::endl;
        m_elapsed = 0;
        m_counter = 0;
    }*/
}


void InputSystem::Receive(Event* event)
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
            //m_disabledComponents[component->GetComponentType()].push_back(component);
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