/**
 * @file     Constructor.hpp
 * @author   Marek Cernak
 * @date     3/24/2019
 *
 * @brief Callback to constructor method holder. 
 */

#pragma once
#include "../EngineAPI.hpp"

namespace engine 
{
    namespace common
    {
        /**
        * Interface Constructor class used to call a specific Constructor with varying count/types of parameters.
        */
        template<typename T>
        class ENGINE_API IBaseConstructor
        {
        public:

            /**
            * Calls a specific Constructor object with varying count/types of parameters.
            * object [in/out] Object of type T to allocate memory for and create, only create otherwise.
            * params [in] Create method callback function parameters.
            * Constructor - specific type of a constructor to call with given parameters.
            * @return pointer to a newly created/reconstructed object.
            */
            template<typename Constructor, typename ...Params>
            T* Construct(T* object, Params&&... params)
            {
                return static_cast<Constructor*>(this)->Construct(object, std::forward<Params>(params)...);
            }
        };

        template<typename T, typename ...Params>
        class ENGINE_API BaseConstructor : public IBaseConstructor<T>
        {
        public:

            template<typename ...Params>
            BaseConstructor(const std::function<T*(T*, Params...)>& constructor) : m_constructor(constructor)
            {
            }

            /**
            * Calls a stored callback to create function with provided parameters.
            * object [in/out] Object of type T to allocate memory for and create, only create otherwise.
            * params [in] Create method callback function parameters.
            * @return pointer to a newly created/reconstructed object.
            */
            template<typename ...Params>
            T* Construct(T* object, Params&&... params)
            {
                return m_constructor(object, std::forward<Params>(params)...);
            }

            std::function<T*(T*, Params...)> m_constructor;
        };
    }
}