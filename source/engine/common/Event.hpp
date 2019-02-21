#pragma once
#include "../EntityManager.hpp"
#include "../components/MovementComponent.hpp"
#include "SDL.h"
#include "Enums.hpp"

class ENGINE_API Message
{
public:
    template<typename T>
    T* GetMessage()
    {
        return static_cast<T*>(this);
    }
    MessageType m_type;
};

class ENGINE_API Event
{
public:
    EntityManager::Entity m_entity;
    MessageType m_type;
    Message* m_message;
};

class ENGINE_API Move : public Message
{
public:
    MovementType m_movement;
};

class ENGINE_API ChangePoisition : public Message
{
public:
    double m_positionX;
    double m_positionY;
};