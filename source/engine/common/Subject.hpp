/**
 * @file     Subject.hpp
 * @author   Marek Cernak
 * @date     3/24/2019
 *
 * @brief Subject declaration.
 */

#pragma once
#include <vector>
#include <functional>
#include "../EngineAPI.hpp"

namespace engine
{
    namespace common
    {
        class Event;
        typedef std::function<void(Event* event)> OnNotifyCallback;
        
        /**
         * Subject class providing interface for observers to subscribe to.
         */
        class ENGINE_API Subject {

        public:

            /**
            * Subscribes observer callback function.
            * observer [in] Observer to subscribe to this subject.
            */
            void addObserver(const OnNotifyCallback& observer);
            
            /**
            * Removses observer.
            * observer [in] Observer to remove from this subject.
            */
            void removeObserver(const OnNotifyCallback& observer);
            
            /**
            * Notifies subscribed observers.
            * observer [in] Event to notify observers with.
            */
            void notify(Event* event);

        private:
            std::vector<OnNotifyCallback> observers;
        };
    }
}