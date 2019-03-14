#pragma once
#include "../EngineAPI.hpp"
#include <unordered_map>
#include <functional>
#include <SDL.h>

class Event;
class InputComponent;
class ENGINE_API InputHandler
{
    typedef std::function<Event*(InputComponent*, std::unordered_map<SDL_Keycode, bool>&, Uint32)> Handler;
public:

    void RegisterHandler(SDL_Keycode keycode, const Handler& handler);
    Event* HandleInput(SDL_Keycode keycode, InputComponent* inputComponent, std::unordered_map<SDL_Keycode, bool>& keycodes, Uint32 eventType);

private:
    std::unordered_map<SDL_Keycode, Handler> m_keyHandlers;
};