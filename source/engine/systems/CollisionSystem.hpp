/**
 * @file     CollisionSystem.hpp
 * @author   Marek Cernak
 * @date     3/25/2019
 */

#pragma once
#include "../EngineAPI.hpp"
#include "System.hpp"
#include "../handlers/EventHandler.hpp"
#include "../managers/EntityManager.hpp"

namespace engine
{
    namespace components
    {
        class CollisionComponent;
    }
    namespace systems
    {
        /**
         * Collision system class used to update register, create and update the collision components.
         */
        class ENGINE_API CollisionSystem : public System
        {
        public:

            CollisionSystem(handlers::EventHandler<CollisionSystem, components::CollisionComponent>* eventHandler);
            virtual void Update() override;
            virtual void Destroy() override;
            virtual void Receive(common::Event* event) override;

            /**
             * Registers subtype of the CollisionComponent creation callback.
             * componentType [in] Type of the component.
             * constructor [in] Component creation callback to register.
             */
            template<typename ...Params>
            void RegisterComponent(ComponentType componentType, const std::function<components::CollisionComponent*(components::CollisionComponent*, Params...)>& constructor)
            {
                m_baseConstructors[componentType] = new common::BaseConstructor<components::CollisionComponent, Params...>(constructor);
            }

            /**
             * Creates new CollisionComponent and stores it to the container of components to update.
             * componentType [in] Type of the component (subtype of CollisionComponent) to create.
             * params [in] Parameters to pass to creation callback.
             */
            template<typename ...Params>
            void CreateComponent(ComponentType componentType, Params&&... params)
            {
                components::CollisionComponent* newComponent = nullptr;
                bool isNew = true;
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
                components::CollisionComponent* component = m_baseConstructors[componentType]->Construct<common::BaseConstructor<components::CollisionComponent, Params...>>(newComponent, std::forward<Params>(params)...);
                if (component)
                {
                    if (isNew)
                    {
                        m_components.push_back(component);
                    }
                    m_componentLookUp[component->GetEntity()] = component;
                }
            }

        protected:

            virtual void DisableComponents(int excludeEntity);

        private:

            std::vector<components::CollisionComponent*> m_components;
            std::unordered_map<managers::EntityManager::Entity, components::CollisionComponent*> m_componentLookUp;
            handlers::EventHandler<CollisionSystem, components::CollisionComponent>* m_eventHandler;
            std::unordered_map<ComponentType, common::IBaseConstructor<components::CollisionComponent>*> m_baseConstructors;
        };
    }
}