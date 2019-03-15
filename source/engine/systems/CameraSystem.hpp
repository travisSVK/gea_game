#pragma once
#include "../EngineAPI.hpp"
#include "System.hpp"
#include "../handlers/EventHandler.hpp"
#include "../managers/EntityManager.hpp"

class CameraComponent;
class ENGINE_API CameraSystem : public System
{
public:

    CameraSystem(EventHandler<CameraSystem, CameraComponent>* eventHandler);
    virtual void Update() override;
    virtual void Destroy() override;
    void Receive(Event* event);

    template<typename ...Params>
    void RegisterComponent(ComponentType componentType, std::function<CameraComponent*(CameraComponent*, Params...)> constructor)
    {
        m_baseConstructors[componentType] = new BaseConstructor<CameraComponent, Params...>(constructor);
    }

    template<typename ...Params>
    void CreateComponent(ComponentType componentType, Params&&... params)
    {
        CameraComponent* newComponent = nullptr;
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
        CameraComponent* component = m_baseConstructors[componentType]->Construct<BaseConstructor<CameraComponent, Params...>>(newComponent, std::forward<Params>(params)...);
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

    std::vector<CameraComponent*> m_components;
    std::unordered_map<ComponentType, std::vector<CameraComponent*>> m_disabledComponents;
    std::unordered_map<EntityManager::Entity, CameraComponent*> m_componentLookUp;
    EventHandler<CameraSystem, CameraComponent>* m_eventHandler;
    std::unordered_map<ComponentType, IBaseConstructor<CameraComponent>*> m_baseConstructors;
};