#pragma once
#include "System.hpp"
#include <vector>
#include <unordered_map>
#include "../handlers/EventHandler.hpp"
#include "../common/Constructor.hpp"

class MovementComponent;
class ENGINE_API MovementSystem : public System
{
public:


    MovementSystem(EventHandler<MovementSystem, MovementComponent>* eventHandler);
    virtual void Update(float delta) override;
    virtual void Destroy() override;
    void Receive(Event* event);

    void Register(MovementComponentType componentType)
    {
        m_constructors[componentType] = new Constructor();
    }

    template<typename T, typename ...Params>
    void CreateComponent(MovementComponentType renderType, Params&&... params)
    {
        T* obj = m_constructors.at(renderType)->Construct<T>(std::forward<Params>(params)...);
        if (obj)
        {
            m_components.push_back(obj);
            m_componentLookUp[obj->GetEntity()] = obj;
        }
    }

private:

    std::vector<MovementComponent*> m_components;
    std::unordered_map<EntityManager::Entity, MovementComponent*> m_componentLookUp;
    std::unordered_map<MovementComponentType, Constructor*> m_constructors;
    EventHandler<MovementSystem, MovementComponent>* m_eventHandler;
};