#include "CollisionSystem.hpp"
#include "../common/Event.hpp"
#include "../components/CollisionComponent.hpp"
#include <algorithm>

CollisionSystem::CollisionSystem(EventHandler<CollisionSystem, CollisionComponent>* eventHandler) : m_eventHandler(eventHandler)
{
    m_components.reserve(1000);
    m_componentLookUp.reserve(1000);
}

void CollisionSystem::Update()
{
    if (m_enabled)
    {
        std::vector<CollisionComponent *> components;
        components.reserve(m_components.size());
        std::copy_if(m_components.begin(), m_components.end(), std::back_inserter(components), [](auto& collisionComponent) {return collisionComponent->IsEnabled(); });
        auto componentsSize = components.size();
        for (int i = componentsSize - 1; i >= 0; --i)
        {
            CollisionComponent* component = components.back();
            components.pop_back();
            if (component->IsEnabled())
            {
                std::vector<Event*> events = component->Update(components);
                for (auto event : events)
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
}

void CollisionSystem::Receive(Event* event)
{
    if (m_enabled)
    {
        m_eventHandler->HandleEvent(this, m_componentLookUp, event);
    }
}

void CollisionSystem::DisableComponents(int excludeEntity)
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

void CollisionSystem::Destroy()
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