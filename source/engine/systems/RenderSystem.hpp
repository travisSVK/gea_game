#pragma once
#include "System.hpp"
#include "../handlers/EventHandler.hpp"
#include "../managers/EntityManager.hpp"

namespace engine
{
    namespace components
    {
        class RenderComponent;
    }
    namespace systems
    {
        class ENGINE_API RenderSystem : public System
        {
        public:

            RenderSystem(handlers::EventHandler<RenderSystem, components::RenderComponent>* eventHandler);
            virtual void Update() override;
            virtual void Destroy() override;
            void Receive(common::Event* event);

            template<typename ...Params>
            void RegisterComponent(ComponentType componentType, std::function<components::RenderComponent*(components::RenderComponent*, Params...)> constructor)
            {
                m_baseConstructors[componentType] = new common::BaseConstructor<components::RenderComponent, Params...>(constructor);
            }

            template<typename ...Params>
            void CreateComponent(ComponentType componentType, Params&&... params)
            {
                components::RenderComponent* newComponent = nullptr;
                bool isNew = true;
                if (componentType == StaticRender)
                {
                    for (auto component : m_staticComponents)
                    {
                        if ((component->GetComponentType() == componentType) && !component->IsEnabled())
                        {
                            newComponent = component;
                            m_componentLookUp.erase(newComponent->GetEntity());
                            isNew = false;
                            break;
                        }
                    }
                }
                else
                {
                    for (auto component : m_components)
                    {
                        if ((component->GetComponentType() == componentType) && !component->IsEnabled())
                        {
                            newComponent = component;
                            m_componentLookUp.erase(newComponent->GetEntity());
                            isNew = false;
                            break;
                        }
                    }
                }

                components::RenderComponent* component = m_baseConstructors[componentType]->Construct<common::BaseConstructor<components::RenderComponent, Params...>>(newComponent, std::forward<Params>(params)...);
                if (component)
                {
                    if (isNew)
                    {
                        if (componentType == StaticRender)
                        {
                            m_staticComponents.push_back(component);
                        }
                        else
                        {
                            m_components.push_back(component);
                        }
                    }
                    m_componentLookUp[component->GetEntity()] = component;
                }
            }
            virtual void DisableComponents(int excludeEntity);

        private:

            std::vector<components::RenderComponent*> m_components;
            std::vector<components::RenderComponent*> m_staticComponents;
            std::unordered_map<managers::EntityManager::Entity, components::RenderComponent*> m_componentLookUp;
            handlers::EventHandler<RenderSystem, components::RenderComponent>* m_eventHandler;
            std::unordered_map<ComponentType, common::IBaseConstructor<components::RenderComponent>*> m_baseConstructors;
        };

    }
}