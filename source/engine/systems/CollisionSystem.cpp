#include "CollisionSystem.hpp"
#include "../common/Event.hpp"
#include "../components/CollisionComponent.hpp"
#include <algorithm>

namespace engine
{
    namespace systems
    {
        CollisionSystem::CollisionSystem(handlers::EventHandler<CollisionSystem, components::CollisionComponent>* eventHandler) : m_eventHandler(eventHandler)
        {
            m_components.reserve(1000);
            m_componentLookUp.reserve(1000);
        }

        void CollisionSystem::Update()
        {
            if (m_enabled)
            {
                std::vector<components::CollisionComponent*> components;
                components.reserve(m_components.size());
                std::copy_if(m_components.begin(), m_components.end(), std::back_inserter(components), [](auto& collisionComponent) {return collisionComponent->IsEnabled(); });
                auto componentsSize = components.size();
                for (int i = componentsSize - 1; i >= 0; --i)
                {
                    components::CollisionComponent* component = components.back();
                    components.pop_back();
                    if (component->IsEnabled())
                    {
                        std::vector<common::Event*> events = component->Update(components);
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

        void CollisionSystem::Receive(common::Event* event)
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
    }
}