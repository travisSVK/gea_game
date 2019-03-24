#include "InputHandler.hpp"

namespace engine
{
    namespace handlers
    {
        void InputHandler::RegisterHandler(SDL_Keycode keycode, const Handler& handler)
        {
            m_keyHandlers[keycode] = handler;
        }

        common::Event* InputHandler::HandleInput(SDL_Keycode keycode, components::InputComponent* inputComponent, std::unordered_map<SDL_Keycode, bool>& keycodes, Uint32 eventType)
        {
            if (m_keyHandlers.find(keycode) != m_keyHandlers.end())
            {
                return m_keyHandlers[keycode](inputComponent, keycodes, eventType);
            }
            return nullptr;
        }
    }
}
