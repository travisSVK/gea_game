#pragma once
#include "../common/Subject.hpp"
#include "../common/Enums.hpp"
#include <unordered_map>
#include "../common/Constructor.hpp"
#include "../managers/EntityManager.hpp"


class ENGINE_API System
{
public:

    System();
    virtual ~System() = default;
    virtual void Update() = 0;
    virtual void Destroy() = 0;
    virtual void Subscribe(const OnNotifyCallback& receiveCallback);
    virtual void Unsubscribe(const OnNotifyCallback& receiveCallback);
    virtual void Receive(Event* event) = 0;
    virtual void Disable(int excludeEntity);
    virtual void Enable();

protected:

    virtual void Notify(Event* event);
    virtual void DisableComponents(int excludeEntity) = 0;

protected:

    bool m_enabled;

private:
    Subject* m_subject;
};