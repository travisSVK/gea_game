#pragma once
#include "System.hpp"
#include "../handlers/EventHandler.hpp"
#include "../managers/EntityManager.hpp"

class RenderComponent;
class ENGINE_API RenderSystem : public System
{
public:

    RenderSystem(EventHandler<RenderSystem, RenderComponent>* eventHandler);
    virtual void Update() override;
    virtual void Destroy() override;
    void Receive(Event* event);

    template<typename ...Params>
    void RegisterComponent(ComponentType componentType, std::function<RenderComponent*(RenderComponent*, Params...)> constructor)
    {
        m_baseConstructors[componentType] = new BaseConstructor<RenderComponent, Params...>(constructor);
    }

    template<typename ...Params>
    void CreateComponent(ComponentType componentType, Params&&... params)
    {
        RenderComponent* newComponent = nullptr;
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
        
        /*if (m_disabledComponents[componentType].size() > 0)
        {
            newComponent = m_disabledComponents[componentType].front();
            m_disabledComponents[componentType].erase(m_disabledComponents[componentType].begin());
            m_componentLookUp.erase(newComponent->GetEntity());
            isNew = false;
        }*/
        RenderComponent* component = m_baseConstructors[componentType]->Construct<BaseConstructor<RenderComponent, Params...>>(newComponent, std::forward<Params>(params)...);
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

    std::vector<RenderComponent*> m_components;
    std::vector<RenderComponent*> m_staticComponents;
    std::unordered_map<ComponentType, std::vector<RenderComponent*>> m_disabledComponents;
    std::unordered_map<EntityManager::Entity, RenderComponent*> m_componentLookUp;
    EventHandler<RenderSystem, RenderComponent>* m_eventHandler;
    std::unordered_map<ComponentType, IBaseConstructor<RenderComponent>*> m_baseConstructors;
};
