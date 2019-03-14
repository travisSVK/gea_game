#include "MovementSystem.hpp"
#include "../common/Event.hpp"
#include "../components/MovementComponent.hpp"
#include <chrono>
#include <iostream>

MovementSystem::MovementSystem(EventHandler<MovementSystem, MovementComponent>* eventHandler) : m_eventHandler(eventHandler), System()
{
    m_components.reserve(100);
    m_componentLookUp.reserve(100);
    m_counter = 0;
    m_elapsed = 0.0;
}

void MovementSystem::Update()
{
    //m_counter++;
    
    if (m_enabled)
    {
        for (auto component : m_components)
        {
            if (component->IsEnabled())
            {
                std::vector<Event*> events = component->Update();
                for (auto event : events)
                {
                    event->m_entity = component->GetEntity();
                    //auto start = std::chrono::high_resolution_clock::now();
                    Notify(event);
                    //auto finish = std::chrono::high_resolution_clock::now();
                    //std::chrono::duration<double> elapsed = finish - start;
                    //m_elapsed += elapsed.count();
                    EventPool::AddEvent(event);
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
    /*if (m_counter == 100)
    {
        std::cout << "MovementSystem: " << m_elapsed << std::endl;
        m_elapsed = 0;
        m_counter = 0;
    }*/
}


void MovementSystem::Receive(Event* event)
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
            component->Disable();
            //m_disabledComponents[component->GetComponentType()].push_back(component);
        }
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