/**
 * @file     Subject.cpp
 * @author   Marek Cernak
 * @date     3/24/2019
 *
 * @brief Subject definition.
 */

#include <algorithm>
#include "Subject.hpp"
#include "Event.hpp"

namespace engine
{
    namespace common
    {
        void Subject::addObserver(const OnNotifyCallback& observer)
        {
            observers.push_back(observer);
        }

        void Subject::removeObserver(const OnNotifyCallback& observer)
        {
            observers.erase(std::remove_if(observers.begin(),
                observers.end(),
                [&](const OnNotifyCallback& obs) {return obs.target<OnNotifyCallback>() == observer.target<OnNotifyCallback>(); }));
        }

        void Subject::notify(Event* event)
        {
            for (const auto& observer : observers)
            {
                observer(event);
            }
        }
    }
}