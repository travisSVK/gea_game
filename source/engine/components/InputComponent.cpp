#include "InputComponent.hpp"
#include "../common/Event.hpp"
#include "../handlers/InputHandler.hpp"

namespace engine
{
    namespace components
    {
        void InputComponent::Create(managers::EntityManager::Entity entity, handlers::InputHandler* inputHandler, const std::vector<SDL_Keycode>& keycodes)
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
            m_temporaryDisabled = false;
        }

        common::Event* InputComponent::Update(const SDL_Event& event)
        {

            common::Event* message = nullptr;
            if (!m_temporaryDisabled)
            {
                message = m_inputHandler->HandleInput(event.key.keysym.sym, this, m_keyCodes, event.type);
            }
            return message;
        }

        void InputComponent::Receive(common::Event* message)
        {
        }

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

        managers::EntityManager::Entity InputComponent::GetEntity()
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
    }
}