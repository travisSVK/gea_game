/**
 * @file     RenderSystem.hpp
 * @author   Marek Cernak
 * @date     3/25/2019
 */

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
        /**
         * Render system class used to update register, create and update the render components.
         */
        class ENGINE_API RenderSystem : public System
        {
        public:

            RenderSystem(handlers::EventHandler<RenderSystem, components::RenderComponent>* eventHandler);
            virtual void Update() override;
            virtual void Destroy() override;
            void Receive(common::Event* event);

            /**
             * Registers subtype of the RenderComponent creation callback.
             * componentType [in] Type of the component.
             * constructor [in] Component creation callback to register.
             */
            template<typename ...Params>
            void RegisterComponent(ComponentType componentType, const std::function<components::RenderComponent*(components::RenderComponent*, Params...)>& constructor)
            {
                m_baseConstructors[componentType] = new common::BaseConstructor<components::RenderComponent, Params...>(constructor);
            }

            /**
             * Creates new RenderComponent and stores it to the container of components to update.
             * componentType [in] Type of the component (subtype of RenderComponent) to create.
             * params [in] Parameters to pass to creation callback.
             */
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