#include "HorizontalCameraComponent.hpp"
#include "common/Event.hpp"

namespace game
{
    namespace components
    {
        void HorizontalCameraComponent::Create(engine::managers::EntityManager::Entity entity, const glm::dvec2& position, const std::vector<engine::managers::EntityManager::Entity>& excludedEntities)
        {
            CameraComponent::Create(entity, position, excludedEntities);
        }

        engine::common::Event* HorizontalCameraComponent::Update()
        {
            engine::common::MoveUp* moveUp = nullptr;
            if (m_positionChange.y != 0.0)
            {
                engine::common::Event* event = engine::common::EventPool::GetEvent(MOVE_UP);
                if (event)
                {
                    moveUp = event->GetMessage<engine::common::MoveUp>();
                }
                else
                {
                    moveUp = new engine::common::MoveUp();
                }
                moveUp->m_type = MessageType::MOVE_UP;
                moveUp->m_moveLength = m_positionChange.y;
                moveUp->m_excludedEntities = m_excludedEntities;
                m_positionChange = glm::dvec2(0.0, 0.0);
            }
            return moveUp;
        }

        void HorizontalCameraComponent::Receive(engine::common::Event* message)
        {
            CameraComponent::Receive(message);
        }

        ComponentType HorizontalCameraComponent::GetComponentType()
        {
            return HorizontalCamera;
        }

        void HorizontalCameraComponent::Destroy()
        {}
    }
}