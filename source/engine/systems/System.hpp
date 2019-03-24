#pragma once
#include "../common/Subject.hpp"
#include "../common/Enums.hpp"
#include <unordered_map>
#include "../common/Constructor.hpp"
#include "../managers/EntityManager.hpp"

namespace engine
{
    namespace systems
    {
        class ENGINE_API System
        {
        public:

            System();
            virtual ~System() = default;
            virtual void Update() = 0;
            virtual void Destroy() = 0;
            virtual void Subscribe(const common::OnNotifyCallback& receiveCallback);
            virtual void Unsubscribe(const common::OnNotifyCallback& receiveCallback);
            virtual void Receive(common::Event* event) = 0;
            virtual void Disable(int excludeEntity);
            virtual void Enable();

        protected:

            virtual void Notify(common::Event* event);
            virtual void DisableComponents(int excludeEntity) = 0;

        protected:

            bool m_enabled;

        private:
            common::Subject* m_subject;
        };
    }
}