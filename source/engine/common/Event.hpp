#pragma once
#include "../managers/EntityManager.hpp"
#include "SDL.h"
#include "Enums.hpp"
#include "GLM/glm.hpp"
#include <vector>
#include <unordered_map>

class ENGINE_API Event
{
public:
    template<typename T>
    T* GetMessage()
    {
        return static_cast<T*>(this);
    }
    EntityManager::Entity m_entity;
    MessageType m_type;
};

class ENGINE_API EventPool
{
public:
    static Event* GetEvent(MessageType type);
    static void AddEvent(Event* event);
    static void DestroyEvents();

private:
    static std::unordered_map<MessageType, std::vector<Event*>> m_eventPool;
};

class ENGINE_API Move : public Event
{
public:
    MovementType m_movement;
};

class ENGINE_API ChangePoisition : public Event
{
public:
    glm::dvec2 m_position;
    glm::dvec2 m_absolutePosition;
    MovementType m_movement;
};

class ENGINE_API CreateProjectile : public Event
{
public:
    char* m_projetileSprites;
    EntityManager::Entity m_newEntity;
};

class ENGINE_API Crouch : public Event
{
};

class ENGINE_API CollisionHappened : public Event
{
public:
    EntityManager::Entity m_firstColEntity;
    EntityManager::Entity m_secondColEntity;
    ComponentType m_firstColComponentType;
    ComponentType m_secondColComponentType;
    DirectionType m_firstColDirection;
    DirectionType m_secondColDirection;
};

class ENGINE_API MoveUp : public Event
{
public:
    std::vector<EntityManager::Entity> m_excludedEntities;
    float m_moveLength;
};

class ENGINE_API ExcludeEntity : public Event
{
public:
    EntityManager::Entity m_cameraEntity;
    EntityManager::Entity m_excludedEntity;
};

class ENGINE_API IncludeEntity : public Event
{
public:
    EntityManager::Entity m_cameraEntity;
    EntityManager::Entity m_includedEntity;
};

class ENGINE_API MoveElevator : public Event
{
public:
    std::vector<EntityManager::Entity> m_elevatorEntities;
    float m_moveLength;
};

class ENGINE_API PlayerEliminated : public Event
{
};

class ENGINE_API PlayerFinished : public Event
{
};

class ENGINE_API DriveCar : public Event
{
};

class ENGINE_API OpenDoor : public Event
{
};

class ENGINE_API PlayerHidden : public Event
{
public:
    EntityManager::Entity m_playerEntity;
};

class ENGINE_API SpawnEnemy : public Event
{
public:
    glm::dvec2 m_position;
};

class ENGINE_API CreateEnemy : public Event
{
public:
    EntityManager::Entity m_newEntity;
    glm::dvec2 m_position;
};

class ENGINE_API ExitGame : public Event
{
};