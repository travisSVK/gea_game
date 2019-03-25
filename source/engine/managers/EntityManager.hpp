/**
 * @file     EntityManager.hpp
 * @author   Marek Cernak
 * @date     3/24/2019
 */

#pragma once
#include "../EngineAPI.hpp"

namespace engine
{
    namespace managers
    {
        /**
         * Entity manager class responsible for creation of new entities.
         */
        class ENGINE_API EntityManager
        {
        public:
            typedef unsigned int Entity;

            EntityManager();

            /**
             * Creates new Entity.
             * @return Entity.
             */
            const Entity CreateEntity();

        private:

            Entity m_currentId;
        };
    }
}