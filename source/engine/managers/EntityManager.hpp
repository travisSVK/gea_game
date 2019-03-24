#pragma once
#include "../EngineAPI.hpp"

namespace engine
{
    namespace managers
    {
        class ENGINE_API EntityManager
        {
        public:
            typedef unsigned int Entity;

            EntityManager();
            const Entity CreateEntity();

        private:

            Entity m_currentId;
        };
    }
}