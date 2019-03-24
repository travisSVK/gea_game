#include "RenderSystem.hpp"
#include "../common/Event.hpp"
#include "../components/RenderComponent.hpp"
#include "../managers/RenderManager.hpp"

namespace engine
{
    namespace systems
    {
        RenderSystem::RenderSystem(handlers::EventHandler<RenderSystem, components::RenderComponent>* eventHandler) : m_eventHandler(eventHandler), System()
        {
            m_components.reserve(1000);
            m_componentLookUp.reserve(1000);
        }

        void RenderSystem::Update()
        {
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

        void RenderSystem::Receive(common::Event* event)
        {
            if (m_enabled)
            {
                m_eventHandler->HandleEvent(this, m_componentLookUp, event);
            }
        }

        void RenderSystem::DisableComponents(int excludeEntity)
        {
            for (auto component : m_staticComponents)
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

        void RenderSystem::Destroy()
        {
            for (auto component : m_components)
            {
                component->Destroy();
                delete component;
            }
            managers::RenderManager::DestroyTextures();
            for (const auto& constructor : m_baseConstructors)
            {
                delete constructor.second;
            }
            delete m_eventHandler;
        }

    }
}