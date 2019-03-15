#pragma once
#include "../EngineAPI.hpp"
#include "System.hpp"
#include "../handlers/EventHandler.hpp"
#include "../managers/EntityManager.hpp"

class CollisionComponent;
class ENGINE_API CollisionSystem : public System
{
public:

    CollisionSystem(EventHandler<CollisionSystem, CollisionComponent>* eventHandler);
    virtual void Update() override;
    virtual void Destroy() override;
    void Receive(Event* event);

    template<typename ...Params>
    void RegisterComponent(ComponentType componentType, std::function<CollisionComponent*(CollisionComponent*, Params...)> constructor)
    {
        m_baseConstructors[componentType] = new BaseConstructor<CollisionComponent, Params...>(constructor);
    }

    template<typename ...Params>
    void CreateComponent(ComponentType componentType, Params&&... params)
    {
        CollisionComponent* newComponent = nullptr;
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
       /* if (m_disabledComponents[componentType].size() > 0)
        {
            newComponent = m_disabledComponents[componentType].front();
            m_disabledComponents[componentType].erase(m_disabledComponents[componentType].begin());
            m_componentLookUp.erase(newComponent->GetEntity());
            isNew = false;
        }*/
        CollisionComponent* component = m_baseConstructors[componentType]->Construct<BaseConstructor<CollisionComponent, Params...>>(newComponent, std::forward<Params>(params)...);
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

    std::vector<CollisionComponent*> m_components;
    std::unordered_map<ComponentType, std::vector<CollisionComponent*>> m_disabledComponents;
    std::unordered_map<EntityManager::Entity, CollisionComponent*> m_componentLookUp;
    EventHandler<CollisionSystem, CollisionComponent>* m_eventHandler;
    std::unordered_map<ComponentType, IBaseConstructor<CollisionComponent>*> m_baseConstructors;
};