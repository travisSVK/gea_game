#include "EntityManager.hpp"

namespace engine
{
    namespace managers
    {
        EntityManager::EntityManager() : m_currentId(0) {}

        const EntityManager::Entity EntityManager::CreateEntity()
        {
            return m_currentId++;
        }
    }
}