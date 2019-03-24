#include "components/CameraComponent.hpp"

namespace game
{
    namespace components
    {
        class HorizontalCameraComponent : public engine::components::CameraComponent
        {
        public:

            virtual void Create(engine::managers::EntityManager::Entity entity, const glm::dvec2& position, const std::vector<engine::managers::EntityManager::Entity>& excludedEntities);
            virtual engine::common::Event* Update() override;
            virtual void Destroy() override;
            virtual void Receive(engine::common::Event* message) override;
            virtual ComponentType GetComponentType() override;
        };
    }
}