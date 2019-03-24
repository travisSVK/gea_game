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
        class ENGINE_API InputHandler
        {
            typedef std::function<common::Event*(components::InputComponent*, std::unordered_map<SDL_Keycode, bool>&, Uint32)> Handler;
        public:

            void RegisterHandler(SDL_Keycode keycode, const Handler& handler);
            common::Event* HandleInput(SDL_Keycode keycode, components::InputComponent* inputComponent, std::unordered_map<SDL_Keycode, bool>& keycodes, Uint32 eventType);

        private:
            std::unordered_map<SDL_Keycode, Handler> m_keyHandlers;
        };
    }
}