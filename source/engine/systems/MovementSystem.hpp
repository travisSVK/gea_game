#pragma once
#include "System.hpp"
#include "../handlers/EventHandler.hpp"
#include "../managers/EntityManager.hpp"

class MovementComponent;
class ENGINE_API MovementSystem : public System
{
public:

    MovementSystem(EventHandler<MovementSystem, MovementComponent>* eventHandler);
    virtual void Update() override;
    virtual void Destroy() override;
    void Receive(Event* event);

    template<typename ...Params>
    void RegisterComponent(ComponentType componentType, std::function<MovementComponent*(MovementComponent*, Params...)> constructor)
    {
        m_baseConstructors[componentType] = new BaseConstructor<MovementComponent, Params...>(constructor);
    }

    template<typename ...Params>
    void CreateComponent(ComponentType componentType, Params&&... params)
    {
        MovementComponent* newComponent = nullptr;
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
        /*if (m_disabledComponents[componentType].size() > 0)
        {
            newComponent = m_disabledComponents[componentType].front();
            m_disabledComponents[componentType].erase(m_disabledComponents[componentType].begin());
            m_componentLookUp.erase(newComponent->GetEntity());
            isNew = false;
        }*/
        MovementComponent* component = m_baseConstructors[componentType]->Construct<BaseConstructor<MovementComponent, Params...>>(newComponent, std::forward<Params>(params)...);
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

    std::vector<MovementComponent*> m_components;
    std::unordered_map<ComponentType, std::vector<MovementComponent*>> m_disabledComponents;
    std::unordered_map<EntityManager::Entity, MovementComponent*> m_componentLookUp;
    EventHandler<MovementSystem, MovementComponent>* m_eventHandler;
    std::unordered_map<ComponentType, IBaseConstructor<MovementComponent>*> m_baseConstructors;
};