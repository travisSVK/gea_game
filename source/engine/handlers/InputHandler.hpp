/**
 * @file     InputHandler.hpp
 * @author   Marek Cernak
 * @date     3/24/2019
 */

#pragma once
#include "../EngineAPI.hpp"
#include <unordered_map>
#include <functional>
#include <SDL.h>

namespace engine
{
    namespace common
    {
        class Event;
    }
    namespace components
    {
        class InputComponent;
    }
    namespace handlers
    {
        /**
         * Input handler class used to handle the input events.
         */
        class ENGINE_API InputHandler
        {
            typedef std::function<common::Event*(components::InputComponent*, std::unordered_map<SDL_Keycode, bool>&, Uint32)> Handler;
        public:

            /**
             * Registers new handler for the specific input keycode.
             * keycode [in] Type of keycode to be handled by the handler.
             * handler [in] Handler function to be called when the input event of keycode type is received.
             */
            void RegisterHandler(SDL_Keycode keycode, const Handler& handler);
            
            /**
             * Handles the input event by calling the handler for the keycode type.
             * keycode [in] Type of keycode to be handled.
             * inputComponent [in] Input component that received the input event.
             * keycodes [in/out] Keycodes handled by the inputComponent.
             * eventType [in] Type of the event to be handled for the keycode (eg. press/release).
             * @return Event if produced. Nullptr otherwise.
             */
            common::Event* HandleInput(SDL_Keycode keycode, components::InputComponent* inputComponent, std::unordered_map<SDL_Keycode, bool>& keycodes, Uint32 eventType);

        private:
            std::unordered_map<SDL_Keycode, Handler> m_keyHandlers;
        };
    }
}