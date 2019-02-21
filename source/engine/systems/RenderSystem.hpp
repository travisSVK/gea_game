#pragma once
#include "System.hpp"
#include "../handlers/EventHandler.hpp"
#include <unordered_map>
#include <vector>
#include "../common/Constructor.hpp"

class RenderComponent;
class ENGINE_API RenderSystem : public System
{
public:

    RenderSystem(EventHandler<RenderSystem, RenderComponent>* eventHandler);
    virtual void Update(float delta) override;
    virtual void Destroy() override;
    void Receive(Event* event);

    void Register(RenderComponentType componentType)
    {
        m_constructors[componentType] = new Constructor();
    }

    template<typename T, typename ...Params>
    void CreateComponent(RenderComponentType renderType, Params&&... params)
    {
        T* obj = m_constructors.at(renderType)->Construct<T>(std::forward<Params>(params)...);
        if (obj)
        {
            m_components.push_back(obj);
            m_componentLookUp[obj->GetEntity()] = obj;
        }
    }

private:

    std::vector<RenderComponent*> m_components;
    std::unordered_map<EntityManager::Entity, RenderComponent*> m_componentLookUp;
    std::unordered_map<RenderComponentType, Constructor*> m_constructors;
    EventHandler<RenderSystem, RenderComponent>* m_eventHandler;
};
