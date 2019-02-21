#pragma once
#include "EngineAPI.hpp"

class ENGINE_API EntityManager
{
public:
    typedef unsigned int Entity;

    EntityManager();
    const Entity CreateEntity();

private:

    Entity m_currentId;
};