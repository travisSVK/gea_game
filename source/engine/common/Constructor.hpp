#pragma once
#include "../EngineAPI.hpp"

namespace engine 
{
    namespace common
    {
        template<typename T>
        class ENGINE_API IBaseConstructor
        {
        public:

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
            BaseConstructor(std::function<T*(T*, Params...)> constructor) : m_constructor(constructor)
            {
            }

            template<typename ...Params>
            T* Construct(T* object, Params&&... params)
            {
                return m_constructor(object, std::forward<Params>(params)...);
            }

            std::function<T*(T*, Params...)> m_constructor;
        };

        class ENGINE_API Constructor
        {
        public:
            template<typename T, typename ...Params>
            T* Construct(Params&&... params)
            {
                return new T(std::forward<Params>(params)...);
            }
        };


    }
}