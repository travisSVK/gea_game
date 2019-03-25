/**
 * @file     InputComponent.hpp
 * @author   Marek Cernak
 * @date     3/24/2019
 */

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
        /**
         * Input component class used to process the user input.
         */
        class ENGINE_API InputComponent
        {
        public:
            virtual ~InputComponent() = default;

            /**
             * Creates the input component with specified arguments.
             * entity [in] Entity the component is describing.
             * inputHandler [in] InputHandler to be used to process the input events.
             * keycodes [in] Vector of keycodes to be used by this input component.
             */
            virtual void Create(managers::EntityManager::Entity entity, handlers::InputHandler* inputHandler, const std::vector<SDL_Keycode>& keycodes);
            
            /**
             * Updates the state. Can produce an Event.
             * event [in] Input event to be processed.
             * @return Event produced. Null if no event produced.
             */
            virtual common::Event* Update(const SDL_Event& event);
            
            /**
             * Destroys the component.
             */
            virtual void Destroy();
            
            /**
             * Process the received Event.
             * event [in] Event received to be processed.
             */
            virtual void Receive(common::Event* message);
            
            /**
             * @return Type of the component.
             */
            virtual ComponentType GetComponentType();
            
            /**
             * @return Entity that the component describes.
             */
            managers::EntityManager::Entity GetEntity();
            
            /**
             * @return True if the component is enabled, False otherwise.
             */
            bool IsEnabled();
            
            /**
             * Enables the component.
             */
            void Enable();
            
            /**
             * Disables the component.
             */
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