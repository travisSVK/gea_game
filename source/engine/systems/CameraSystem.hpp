#pragma once
#include "../EngineAPI.hpp"
#include "System.hpp"
#include "../handlers/EventHandler.hpp"
#include "../managers/EntityManager.hpp"

namespace engine
{
    namespace components
    {
        class CameraComponent;
    }
    namespace systems
    {
        class ENGINE_API CameraSystem : public System
        {
        public:

            CameraSystem(handlers::EventHandler<CameraSystem, components::CameraComponent>* eventHandler);
            virtual void Update() override;
            virtual void Destroy() override;
            void Receive(common::Event* event);

            template<typename ...Params>
            void RegisterComponent(ComponentType componentType, std::function<components::CameraComponent*(components::CameraComponent*, Params...)> constructor)
            {
                m_baseConstructors[componentType] = new common::BaseConstructor<components::CameraComponent, Params...>(constructor);
            }

            template<typename ...Params>
            void CreateComponent(ComponentType componentType, Params&&... params)
            {
                components::CameraComponent* newComponent = nullptr;
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
                components::CameraComponent* component = m_baseConstructors[componentType]->Construct<common::BaseConstructor<components::CameraComponent, Params...>>(newComponent, std::forward<Params>(params)...);
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

            std::vector<components::CameraComponent*> m_components;
            std::unordered_map<managers::EntityManager::Entity, components::CameraComponent*> m_componentLookUp;
            handlers::EventHandler<CameraSystem, components::CameraComponent>* m_eventHandler;
            std::unordered_map<ComponentType, common::IBaseConstructor<components::CameraComponent>*> m_baseConstructors;
        };
    }
}