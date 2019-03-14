#include "components/CameraComponent.hpp"

class HorizontalCameraComponent : public CameraComponent
{
public:

    virtual void Create(EntityManager::Entity entity, const glm::dvec2& position, const std::vector<EntityManager::Entity>& excludedEntities);
    virtual Event* Update() override;
    virtual void Destroy() override;
    virtual void Receive(Event* message) override;
    virtual ComponentType GetComponentType() override;
};