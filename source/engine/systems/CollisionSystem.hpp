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
        class ENGINE_API CollisionSystem : public System
        {
        public:

            CollisionSystem(handlers::EventHandler<CollisionSystem, components::CollisionComponent>* eventHandler);
            virtual void Update() override;
            virtual void Destroy() override;
            void Receive(common::Event* event);

            template<typename ...Params>
            void RegisterComponent(ComponentType componentType, const std::function<components::CollisionComponent*(components::CollisionComponent*, Params...)>& constructor)
            {
                m_baseConstructors[componentType] = new common::BaseConstructor<components::CollisionComponent, Params...>(constructor);
            }

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