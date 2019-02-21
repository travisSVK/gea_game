#include "RenderSystem.hpp"
#include "../common/Event.hpp"
#include "../components/RenderComponent.hpp"

RenderSystem::RenderSystem(EventHandler<RenderSystem, RenderComponent>* eventHandler) : m_eventHandler(eventHandler), System()
{
    m_components.reserve(10);
    m_componentLookUp.reserve(10);
}

void RenderSystem::Update(float delta)
{
    for (auto &component : m_components)
    {
        component->Update(delta);
    }
}


void RenderSystem::Receive(Event* event)
{
    m_eventHandler->HandleEvent(this, m_componentLookUp, event);
}

void RenderSystem::Destroy()
{

}