#pragma once
#include "../EngineAPI.hpp"
#include "../managers/EntityManager.hpp"
#include <unordered_map>
#include <SDL.h>
#include "../common/Enums.hpp"

namespace engine
{
    namespace common
    {
        class Event;
    }
    namespace handlers
    {
        class InputHandler;
    }
    namespace components
    {
        class ENGINE_API InputComponent
        {
        public:
            InputComponent() = default;
            virtual ~InputComponent() = default;
            virtual void Create(managers::EntityManager::Entity entity, handlers::InputHandler* inputHandler, const std::vector<SDL_Keycode>& keycodes);
            virtual common::Event* Update(const SDL_Event& event);
            virtual void Destroy();
            virtual void Receive(common::Event* message);
            virtual ComponentType GetComponentType();
            managers::EntityManager::Entity GetEntity();
            bool IsEnabled();
            void Enable();
            void Disable();

        protected:
            managers::EntityManager::Entity m_entity;
            handlers::InputHandler* m_inputHandler;
            std::unordered_map<SDL_Keycode, bool> m_keyCodes;
            bool m_enabled;
            bool m_temporaryDisabled;
        };
    }
}