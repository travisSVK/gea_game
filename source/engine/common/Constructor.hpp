#pragma once
#include "../EngineAPI.hpp"

class ENGINE_API Constructor
{
public:
    template<typename T, typename ...Params>
    T* Construct(Params&&... params) 
    { 
        return new T(std::forward<Params>(params)...); 
    }
};