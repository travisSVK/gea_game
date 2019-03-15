#pragma once
#include "System.hpp"
#include "../EngineAPI.hpp"
#include "../handlers/EventHandler.hpp"
#include "../managers/EntityManager.hpp"

class InputComponent;
class ENGINE_API InputSystem : public System
{
public:

    InputSystem(EventHandler<InputSystem, InputComponent>* eventHandler);
    virtual void Update() override;
    virtual void Destroy() override;
    virtual void Receive(Event* event) override;

    template<typename ...Params>
    void RegisterComponent(ComponentType componentType, std::function<InputComponent*(InputComponent*, Params...)> constructor)
    {
        m_baseConstructors[componentType] = new BaseConstructor<InputComponent, Params...>(constructor);
    }

    template<typename ...Params>
    void CreateComponent(ComponentType componentType, Params&&... params)
    {
        InputComponent* newComponent = nullptr;
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
        InputComponent* component = m_baseConstructors[componentType]->Construct<BaseConstructor<InputComponent, Params...>>(newComponent, std::forward<Params>(params)...);
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

    std::vector<InputComponent*> m_components;
    std::unordered_map<ComponentType, std::vector<InputComponent*>> m_disabledComponents;
    std::unordered_map<EntityManager::Entity, InputComponent*> m_componentLookUp;
    EventHandler<InputSystem, InputComponent>* m_eventHandler;
    std::unordered_map<ComponentType, IBaseConstructor<InputComponent>*> m_baseConstructors;
};