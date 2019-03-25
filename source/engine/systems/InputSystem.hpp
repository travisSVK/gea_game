/**
 * @file     InputSystem.hpp
 * @author   Marek Cernak
 * @date     3/25/2019
 */

#pragma once
#include "System.hpp"
#include "../EngineAPI.hpp"
#include "../handlers/EventHandler.hpp"
#include "../managers/EntityManager.hpp"

namespace engine
{
    namespace components
    {
        class InputComponent;
    }
    namespace systems
    {
        /**
         * Input system class used to update register, create and update the input components.
         */
        class ENGINE_API InputSystem : public System
        {
        public:

            InputSystem(handlers::EventHandler<InputSystem, components::InputComponent>* eventHandler);
            virtual void Update() override;
            virtual void Destroy() override;
            virtual void Receive(common::Event* event) override;

            /**
             * Registers subtype of the InputComponent creation callback.
             * componentType [in] Type of the component.
             * constructor [in] Component creation callback to register.
             */
            template<typename ...Params>
            void RegisterComponent(ComponentType componentType, const std::function<components::InputComponent*(components::InputComponent*, Params...)>& constructor)
            {
                m_baseConstructors[componentType] = new common::BaseConstructor<components::InputComponent, Params...>(constructor);
            }

            /**
             * Creates new InputComponent and stores it to the container of components to update.
             * componentType [in] Type of the component (subtype of InputComponent) to create.
             * params [in] Parameters to pass to creation callback.
             */
            template<typename ...Params>
            void CreateComponent(ComponentType componentType, Params&&... params)
            {
                components::InputComponent* newComponent = nullptr;
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
                components::InputComponent* component = m_baseConstructors[componentType]->Construct<common::BaseConstructor<components::InputComponent, Params...>>(newComponent, std::forward<Params>(params)...);
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

            std::vector<components::InputComponent*> m_components;
            std::unordered_map<managers::EntityManager::Entity, components::InputComponent*> m_componentLookUp;
            handlers::EventHandler<InputSystem, components::InputComponent>* m_eventHandler;
            std::unordered_map<ComponentType, common::IBaseConstructor<components::InputComponent>*> m_baseConstructors;
        };
    }
}