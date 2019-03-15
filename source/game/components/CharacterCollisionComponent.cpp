#include "CharacterCollisionComponent.hpp"
#include "common/Event.hpp"

void CharacterCollisionComponent::Create(EntityManager::Entity entity, const glm::dvec2& position, const glm::ivec2& size, bool isPlayerEntity)
{
    CollisionComponent::Create(entity, position, size);
    m_isPlayerEntity = isPlayerEntity;
    m_temporaryDisabled = false;
    m_crouch = false;
}

std::vector<Event*> CharacterCollisionComponent::Update(const std::vector<CollisionComponent*>& collisionComponents)
{
    std::vector<Event*> messages;
    if (!m_temporaryDisabled)
    {
        for (auto collisionComponent : collisionComponents)
        {
            double ytmp = m_position.y;
            // check with previous y value for player (because it is temporarily changed due to gravity collision check)
            if (m_isPlayerEntity && (collisionComponent->GetComponentType() == ComponentType::ProjectileCollision))
            {
                m_position.y = m_lastPosition.y;
            }
            DirectionType collisionDirection;
            DirectionType otherCollisionDirection;
            if (collisionComponent->CheckCollision(this, otherCollisionDirection, collisionDirection))
            {
                if ((collisionComponent->GetComponentType() == ComponentType::ProjectileCollision) && (!m_isPlayerEntity))
                {
                    m_enabled = false;
                }
                else if (collisionComponent->GetComponentType() == ComponentType::StaticCollision)
                {
                    if ((otherCollisionDirection == DirectionType::DIR_LEFT) || (otherCollisionDirection == DirectionType::DIR_RIGHT))
                    {
                        m_position.x = m_lastPosition.x;
                    }
                    else if (otherCollisionDirection == DirectionType::DIR_UP)
                    {
                        m_position.y = m_lastPosition.y;
                    }
                }
                CollisionHappened* message;
                Event* event = EventPool::GetEvent(COLLISION_HAPPENED);
                if (event)
                {
                    message = event->GetMessage<CollisionHappened>();
                }
                else
                {
                    message = new CollisionHappened();
                }
                message->m_firstColEntity = m_entity;
                message->m_firstColComponentType = ComponentType::CharacterCollision;
                message->m_secondColEntity = collisionComponent->GetEntity();
                message->m_secondColComponentType = collisionComponent->GetComponentType();
                message->m_firstColDirection = collisionDirection;
                message->m_secondColDirection = otherCollisionDirection;
                message->m_type = MessageType::COLLISION_HAPPENED;
                messages.push_back(message);
            }
            // set back the temporarily gravity changed y value in case it was set for projectile test
            m_position.y = ytmp;
        }
        if (m_isPlayerEntity)
        {
            // set back the temporarily gravity changed value to previous one (if there wasnt a collision, camera component will move the player)
            m_position.y = m_lastPosition.y;
        }
    }
    return messages;
}

bool CharacterCollisionComponent::CheckCollision(CollisionComponent* collisionComponent, DirectionType& collisionDirection, DirectionType& collisionDirectionOther)
{
    if (!m_temporaryDisabled && m_enabled && ((collisionComponent->GetComponentType() == ComponentType::StaticCollision) || (collisionComponent->GetComponentType() == ComponentType::ProjectileCollision)))
    {
        double ytmp = m_position.y;
        // check with previous y value for player (because it is temporarily changed due to gravity collision check)
        if (m_isPlayerEntity && (collisionComponent->GetComponentType() == ComponentType::ProjectileCollision))
        {
            m_position.y = m_lastPosition.y;
        }
        if (CollisionComponent::CheckCollision(collisionComponent, collisionDirection, collisionDirectionOther))
        {
            if ((collisionComponent->GetComponentType() == ComponentType::ProjectileCollision) && (!m_isPlayerEntity))
            {
                m_enabled = false;
            }
            else if (collisionComponent->GetComponentType() == ComponentType::StaticCollision)
            {
                if ((collisionDirectionOther == DirectionType::DIR_LEFT) || (collisionDirectionOther == DirectionType::DIR_RIGHT))
                {
                    m_position.x = m_lastPosition.x;
                }
                else if (collisionDirectionOther == DirectionType::DIR_UP)
                {
                    m_position.y = m_lastPosition.y;
                }
            }
            // set back the temporarily gravity changed y value in case it was set for projectile test
            m_position.y = ytmp;
            return true;
        }
        // set back the temporarily gravity changed y value in case it was set for projectile test
        m_position.y = ytmp;
    }
    return false;
}

void CharacterCollisionComponent::Receive(Event* message)
{
    if (message->m_type == MessageType::PLAYER_HIDDEN)
    {
        if (m_temporaryDisabled)
        {
            m_temporaryDisabled = false;
        }
        else
        {
            m_temporaryDisabled = true;
        }
    }
    else if (message->m_type == MessageType::CROUCH)
    {
        if (m_crouch)
        {
            m_crouch = false;
            m_size.y = 40;
            m_position.y -= 10;
            m_lastPosition.y -= 10;
        }
        else
        {
            m_crouch = true;
            m_size.y = 20;
            m_position.y += 10;
            m_lastPosition.y += 10;
        }
    }
}

ComponentType CharacterCollisionComponent::GetComponentType()
{
    return CharacterCollision;
}

void CharacterCollisionComponent::Destroy()
{

}