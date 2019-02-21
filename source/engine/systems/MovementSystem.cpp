#include "MovementSystem.hpp"
#include "../common/Event.hpp"
#include "../components/MovementComponent.hpp"

MovementSystem::MovementSystem(EventHandler<MovementSystem, MovementComponent>* eventHandler) : m_eventHandler(eventHandler), System()
{
    m_components.reserve(10);
    m_componentLookUp.reserve(10);
}

void MovementSystem::Update(float delta)
{
    for (auto &component : m_components)
    {
        double positionX;
        double positionY;
        if (component->Update(delta, positionX, positionY))
        {
            ChangePoisition* changePositionMessage = new ChangePoisition();
            changePositionMessage->m_positionX = positionX;
            changePositionMessage->m_positionY = positionY;
            changePositionMessage->m_type = MessageType::POSITION_CHANGE;

            Event* event = new Event();
            event->m_entity = component->GetEntity();
            event->m_type = MessageType::POSITION_CHANGE;
            event->m_message = changePositionMessage;
            Notify(event);
            delete event;
        }
    }
}


void MovementSystem::Receive(Event* event)
{
    m_eventHandler->HandleEvent(this, m_componentLookUp, event);
}

void MovementSystem::Destroy()
{

}