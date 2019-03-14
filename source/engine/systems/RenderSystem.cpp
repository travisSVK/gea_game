#include "RenderSystem.hpp"
#include "../common/Event.hpp"
#include "../components/RenderComponent.hpp"
#include "../managers/RenderManager.hpp"
#include <chrono>
#include <iostream>

RenderSystem::RenderSystem(EventHandler<RenderSystem, RenderComponent>* eventHandler) : m_eventHandler(eventHandler), System()
{
    m_components.reserve(1000);
    m_componentLookUp.reserve(1000);
    //m_disabledComponents.reserve(100);
    m_counter = 0;
    m_elapsed = 0.0;
}

void RenderSystem::Update()
{
    /*m_counter++;
    auto start = std::chrono::high_resolution_clock::now();*/
    if (m_enabled)
    {
        for (auto component : m_staticComponents)
        {
            if (component->IsEnabled())
            {
                component->Update();
            }
        }
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
    //auto finish = std::chrono::high_resolution_clock::now();
    //std::chrono::duration<double> elapsed = finish - start;
    //m_elapsed += elapsed.count();
    //if (m_counter == 100)
    //{
    //    //std::cout << "RenderSystem: " << m_elapsed << std::endl;
    //    m_elapsed = 0;
    //    m_counter = 0;
    //}
}

void RenderSystem::Receive(Event* event)
{
    /*m_counter++;
    auto start = std::chrono::high_resolution_clock::now();*/
    if (m_enabled)
    {
        m_eventHandler->HandleEvent(this, m_componentLookUp, event);
    }
    /*auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    m_elapsed += elapsed.count();
    if (m_counter == 6300)
    {
        std::cout << "RenderSystem: " << m_elapsed << std::endl;
        m_elapsed = 0;
        m_counter = 0;
    }*/
}

void RenderSystem::DisableComponents(int excludeEntity)
{
    for (auto component : m_staticComponents)
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

void RenderSystem::Destroy()
{
    for (auto component : m_components)
    {
        component->Destroy();
        delete component;
    }
    RenderManager::DestroyTextures();
    for (const auto& constructor : m_baseConstructors)
    {
        delete constructor.second;
    }
    delete m_eventHandler;
}
