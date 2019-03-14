#include "InputComponent.hpp"
#include "../common/Event.hpp"
#include "../handlers/InputHandler.hpp"

void InputComponent::Create(EntityManager::Entity entity, InputHandler* inputHandler, const std::vector<SDL_Keycode>& keycodes)
{
    if (m_inputHandler)
    {
        delete m_inputHandler;
    }
    m_entity = entity;
    m_inputHandler = inputHandler;
    m_enabled = true;
    for (auto keycode : keycodes)
    {
        m_keyCodes[keycode] = false;
    }
}

Event* InputComponent::Update(const SDL_Event& event)
{
    
    Event* message = nullptr;
    message = m_inputHandler->HandleInput(event.key.keysym.sym, this, m_keyCodes, event.type);
    return message;
}

void InputComponent::Receive(Event* message)
{}

void InputComponent::Enable()
{
    m_enabled = true;
}

void InputComponent::Disable()
{
    m_enabled = false;
}

bool InputComponent::IsEnabled()
{
    return m_enabled;
}

EntityManager::Entity InputComponent::GetEntity()
{
    return m_entity;
}

ComponentType InputComponent::GetComponentType()
{
    return BasicInput;
}

void InputComponent::Destroy()
{
    delete m_inputHandler;
}