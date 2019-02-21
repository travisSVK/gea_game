#pragma once
#include "../EntityManager.hpp"
#include "../common/Subject.hpp"
#include "../common/Enums.hpp"

class ENGINE_API System
{
public:

    System();
    virtual ~System() = default;
    virtual void Update(float delta) = 0;
    virtual void Destroy() = 0;
    virtual void Subscribe(const OnNotifyCallback& receiveCallback);
    virtual void Unsubscribe(const OnNotifyCallback& receiveCallback);
    virtual void Receive(Event* event) = 0;

protected:
    
    // notify other systems upon receiving event from component
    virtual void Notify(Event* event);


private:
    Subject* m_subject;
};