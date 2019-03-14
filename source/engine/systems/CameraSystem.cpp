#include "CameraSystem.hpp"
#include "../common/Event.hpp"
#include "../components/CameraComponent.hpp"
#include <iostream>
#include <chrono>

CameraSystem::CameraSystem(EventHandler<CameraSystem, CameraComponent>* eventHandler) : m_eventHandler(eventHandler)
{
    m_components.reserve(2);
    m_componentLookUp.reserve(2);
    m_counter = 0;
    m_elapsed = 0.0;
}

void CameraSystem::Update()
{
    /*m_counter++;
    auto start = std::chrono::high_resolution_clock::now();*/
    if (m_enabled)
    {
        for (auto component : m_components)
        {
            if (component->IsEnabled())
            {
                Event* event = component->Update();
                if (event)
                {
                    event->m_entity = component->GetEntity();
                    Notify(event);
                    EventPool::AddEvent(event);
                    //delete event;
                }
            }
            /*if(!component->IsEnabled())
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
    //auto finish = std::chrono::high_resolution_clock::now();
    //std::chrono::duration<double> elapsed = finish - start;
    //m_elapsed += elapsed.count();
    //if (m_counter == 100)
    //{
    //    //std::cout << "CameraSystem: " << m_elapsed << std::endl;
    //    m_elapsed = 0;
    //    m_counter = 0;
    //}
    
}

void CameraSystem::Receive(Event* event)
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
            //m_disabledComponents[component->GetComponentType()].push_back(component);
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