#pragma once
#include <vector>
#include <functional>
#include "../EngineAPI.hpp"

class Event;
typedef std::function<void(Event* event)> OnNotifyCallback;
class ENGINE_API Subject {

public:
    
    void addObserver(const OnNotifyCallback& observer);
    void removeObserver(const OnNotifyCallback& observer);
    void notify(Event* event);

private:
    std::vector<OnNotifyCallback> observers;
};