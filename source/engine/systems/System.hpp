/**
 * @file     System.hpp
 * @author   Marek Cernak
 * @date     3/25/2019
 */

#pragma once
#include "../common/Subject.hpp"
#include "../common/Enums.hpp"
#include <unordered_map>
#include "../common/Constructor.hpp"
#include "../managers/EntityManager.hpp"

namespace engine
{
    namespace systems
    {
        /**
         * System abstract class used to Subscribe, Unsubscribe and Notify observers.
         */
        class ENGINE_API System
        {
        public:

            System();
            virtual ~System() = default;

            /**
             * Updates all components.
             */
            virtual void Update() = 0;

            /**
             * Destroys all components.
             */
            virtual void Destroy() = 0;

            /**
             * Subscribes observer.
             * receiveCallback [in] Observer as a callback to be called when notifying about the event.
             */
            virtual void Subscribe(const common::OnNotifyCallback& receiveCallback);
            
            /**
             * Unsubscribes observer.
             * receiveCallback [in] Callback previously used to subscribe the observer.
             */
            virtual void Unsubscribe(const common::OnNotifyCallback& receiveCallback);
            
            /**
             * Receives and handles the event.
             * event [in] Event to handle.
             */
            virtual void Receive(common::Event* event) = 0;

            /**
             * Disables the system and its components. Can leave a component enabled when requested.
             * excludeEntity [in] Id of the entity to leave enabled. Set to a value < 0 to disable all components.
             */
            virtual void Disable(int excludeEntity);

            /**
             * Enables the system.
             */
            virtual void Enable();

        protected:

            /**
             * Notifies observers.
             * event [in] Event to notify subscribers about.
             */
            virtual void Notify(common::Event* event);

            /**
             * Disables components. Can leave a component enabled when requested.
             * excludeEntity [in] Id of the entity to leave enabled. Set to a value < 0 to disable all components.
             */
            virtual void DisableComponents(int excludeEntity) = 0;

        protected:

            bool m_enabled;

        private:

            common::Subject* m_subject;
        };
    }
}