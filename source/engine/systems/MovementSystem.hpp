#pragma once
#include "System.hpp"
#include "../handlers/EventHandler.hpp"
#include "../managers/EntityManager.hpp"

namespace engine
{
    namespace components
    {
        class MovementComponent;
    }
    namespace systems
    {
        class ENGINE_API MovementSystem : public System
        {
        public:

            MovementSystem(handlers::EventHandler<MovementSystem, components::MovementComponent>* eventHandler);
            virtual void Update() override;
            virtual void Destroy() override;
            void Receive(common::Event* event);

            template<typename ...Params>
            void RegisterComponent(ComponentType componentType, std::function<components::MovementComponent*(components::MovementComponent*, Params...)> constructor)
            {
                m_baseConstructors[componentType] = new common::BaseConstructor<components::MovementComponent, Params...>(constructor);
            }

            template<typename ...Params>
            void CreateComponent(ComponentType componentType, Params&&... params)
            {
                components::MovementComponent* newComponent = nullptr;
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
                components::MovementComponent* component = m_baseConstructors[componentType]->Construct<common::BaseConstructor<components::MovementComponent, Params...>>(newComponent, std::forward<Params>(params)...);
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

            std::vector<components::MovementComponent*> m_components;
            std::unordered_map<managers::EntityManager::Entity, components::MovementComponent*> m_componentLookUp;
            handlers::EventHandler<MovementSystem, components::MovementComponent>* m_eventHandler;
            std::unordered_map<ComponentType, common::IBaseConstructor<components::MovementComponent>*> m_baseConstructors;
        };
    }
}