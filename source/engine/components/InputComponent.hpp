#pragma once
#include "../EngineAPI.hpp"
#include "../managers/EntityManager.hpp"
#include <unordered_map>
#include <SDL.h>
#include "../common/Enums.hpp"

class InputHandler;
class Event;
class ENGINE_API InputComponent
{
public:
    InputComponent() = default;
    virtual ~InputComponent() = default;
    virtual void Create(EntityManager::Entity entity, InputHandler* inputHandler, const std::vector<SDL_Keycode>& keycodes);
    virtual Event* Update(const SDL_Event& event);
    virtual void Destroy();
    virtual void Receive(Event* message);
    virtual ComponentType GetComponentType();
    EntityManager::Entity GetEntity();
    bool IsEnabled();
    void Enable();
    void Disable();

protected:
    EntityManager::Entity m_entity;
    InputHandler* m_inputHandler;
    std::unordered_map<SDL_Keycode, bool> m_keyCodes;
    bool m_enabled;
};