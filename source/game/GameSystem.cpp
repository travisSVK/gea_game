#include "GameSystem.hpp"
#include "systems/MovementSystem.hpp"
#include "systems/RenderSystem.hpp"
#include "systems/InputSystem.hpp"
#include "systems/CameraSystem.hpp"
#include "systems/CollisionSystem.hpp"

#include "common/Event.hpp"

#include "components/SpritesheetRenderComponent.hpp"
#include "components/SpriteRenderComponent.hpp"
#include "components/StaticRenderComponent.hpp"
#include "components/DoorRenderComponent.hpp"
#include "components/EnemyDoorRenderComponent.hpp"

#include "components/PlayerMovementComponent.hpp"
#include "components/ProjectileMovementComponent.hpp"
#include "components/ElevatorMovementComponent.hpp"
#include "components/AIMovementComponent.hpp"

#include "components/HorizontalCameraComponent.hpp"
#include "components/InputComponent.hpp"

#include "components/ProjectileCollisionComponent.hpp"
#include "components/StaticCollisionComponent.hpp"
#include "components/CharacterCollisionComponent.hpp"

#include "managers/SoundManager.hpp"
#include "managers/RenderManager.hpp"

#include "handlers/EventHandler.hpp"
#include "handlers/InputHandler.hpp"

#include "RandomNumberGenerator.hpp"

using namespace engine::common;
using namespace engine::components;
using namespace engine::systems;
using namespace engine::handlers;
using namespace engine::managers;

namespace game
{
    GameSystem::GameSystem() : System()
    {
        m_systems.reserve(5);
    }

    bool GameSystem::Init(int width, int height)
    {
        SDL_Log("Initializing SDL...\n");

        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL failed the initialization: %s\n", SDL_GetError());
            return false;
        }

        if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 1, 4096) != 0)
        {
            return false;
        }

        //Create window
        m_window = SDL_CreateWindow("Elevator Action", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
        if (m_window == NULL)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
            return false;
        }

        //Create renderer for window
        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
        if (m_renderer == NULL)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
            return false;
        }

        TTF_Init();
        m_font = TTF_OpenFont("data/EA.ttf", 25); //this opens a font style and sets a size
        if (m_font == NULL)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "font cannot be created! SDL_Error: %s\n", SDL_GetError());
            return false;
        }

        //Initialize renderer color
        SDL_SetRenderDrawColor(m_renderer, 75, 75, 75, 255);

        //Clear screen
        SDL_RenderClear(m_renderer);

        SDL_Log("SDL up and running...\n");

        m_gameOver = false;
        return true;
    }

    void GameSystem::Create()
    {
        // input system
        EventHandler<InputSystem, InputComponent>* inputEventHandler = new EventHandler<InputSystem, InputComponent>();
        InputSystem* inputSystem = new InputSystem(inputEventHandler);
        m_systems.push_back(inputSystem);
        m_systemsMap[SystemType::Input] = inputSystem;
        std::function<InputComponent*(InputComponent*, EntityManager::Entity, InputHandler*, const std::vector<SDL_Keycode>&)> inputConstructor =
            [](InputComponent* inputComponent, EntityManager::Entity entity, InputHandler* inputHandler, const std::vector<SDL_Keycode>& keycodes)
        {
            if (!inputComponent)
            {
                InputComponent* newComponent = new InputComponent();
                inputComponent = newComponent;
            }
            inputComponent->Create(entity, inputHandler, keycodes);
            return inputComponent;
        };
        inputSystem->RegisterComponent<EntityManager::Entity, InputHandler*, const std::vector<SDL_Keycode>&>(ComponentType::BasicInput, inputConstructor);
        inputSystem->Subscribe([this](Event* event) {Receive(event); });

        // movement system
        EventHandler<MovementSystem, MovementComponent>* movementEventHandler = new EventHandler<MovementSystem, MovementComponent>();
        movementEventHandler->RegisterHandler(
            MessageType::MOVE,
            [](MovementSystem* movementSystem, const std::unordered_map<EntityManager::Entity, MovementComponent*>& componentsMap, Event* event)
        {
            componentsMap.at(event->m_entity)->Receive(event);
        }
        );
        movementEventHandler->RegisterHandler(
            MessageType::MOVE_UP,
            [](MovementSystem* movementSystem, const std::unordered_map<EntityManager::Entity, MovementComponent*>& componentsMap, Event* event)
        {
            MoveUp* moveUp = event->GetMessage<MoveUp>();
            for (auto const& component : componentsMap)
            {
                MovementComponent* mc = component.second;
                bool excluded = false;
                for (auto excludedEntity : moveUp->m_excludedEntities)
                {
                    if (mc->GetEntity() == excludedEntity)
                    {
                        excluded = true;
                        break;
                    }
                }
                if (!excluded)
                {
                    mc->SetPosition(glm::dvec2(0, -moveUp->m_moveLength));
                }
            }
        }
        );
        movementEventHandler->RegisterHandler(
            MessageType::POSITION_CHANGE,
            [this](MovementSystem* movementSystem, const std::unordered_map<EntityManager::Entity, MovementComponent*>& componentsMap, Event* event)
        {
            if (event->m_entity == m_playerEntity)
            {
                for (auto const& component : componentsMap)
                {
                    component.second->Receive(event);
                }
            }
        }
        );
        movementEventHandler->RegisterHandler(
            MessageType::CREATE_PROJECTILE,
            [this](MovementSystem* movementSystem, const std::unordered_map<EntityManager::Entity, MovementComponent*>& componentsMap, Event* event)
        {
            SoundManager::PlaySFXMusic(m_shoot);
            CreateProjectile* createProjectile = event->GetMessage<CreateProjectile>();
            MovementComponent* mc = componentsMap.at(event->m_entity);
            double posOffset = 0;
            if ((mc->GetMovement() == MovementType::CROUCHING_LEFT) || (mc->GetMovement() == MovementType::CROUCHING_RIGHT))
            {
                posOffset = 25;
            }
            else
            {
                posOffset = 15;
            }
            movementSystem->CreateComponent(
                ComponentType::ProjectileMovement,
                createProjectile->m_newEntity,
                glm::dvec2(mc->GetPosition().x, mc->GetPosition().y + posOffset),
                mc->GetDirection());
        }
        );
        movementEventHandler->RegisterHandler(
            MessageType::CREATE_ENEMY,
            [this](MovementSystem* movementSystem, const std::unordered_map<EntityManager::Entity, MovementComponent*>& componentsMap, Event* event)
        {
            CreateEnemy* createEnemy = event->GetMessage<CreateEnemy>();
            DirectionType dir;
            if (createEnemy->m_position.x < 370)
            {
                dir = DirectionType::DIR_RIGHT;
            }
            else
            {
                dir = DirectionType::DIR_LEFT;
            }
            movementSystem->CreateComponent(
                ComponentType::AIMovement,
                createEnemy->m_newEntity,
                createEnemy->m_position,
                m_playerPosition,
                dir,
                m_entityManager);
        }
        );

        movementEventHandler->RegisterHandler(
            MessageType::COLLISION_HAPPENED,
            [this](MovementSystem* movementSystem, const std::unordered_map<EntityManager::Entity, MovementComponent*>& componentsMap, Event* event)
        {
            CollisionHappened* collisionHappened = event->GetMessage<CollisionHappened>();
            if (collisionHappened->m_firstColComponentType != ComponentType::StaticCollision)
            {
                if (!((collisionHappened->m_firstColEntity == m_playerEntity) && (collisionHappened->m_firstColComponentType == ComponentType::CharacterCollision) &&
                    (collisionHappened->m_secondColComponentType == ComponentType::ProjectileCollision)))
                {
                    componentsMap.at(collisionHappened->m_firstColEntity)->Receive(collisionHappened);
                }
            }
            if (collisionHappened->m_secondColComponentType != ComponentType::StaticCollision)
            {
                if (!((collisionHappened->m_secondColEntity == m_playerEntity) && (collisionHappened->m_secondColComponentType == ComponentType::CharacterCollision) &&
                    (collisionHappened->m_firstColComponentType == ComponentType::ProjectileCollision)))
                {
                    // swap the order so the movement component will always treat itself as the first entity in the message
                    EntityManager::Entity tmp = collisionHappened->m_secondColEntity;
                    ComponentType tmpc = collisionHappened->m_secondColComponentType;
                    collisionHappened->m_secondColEntity = collisionHappened->m_firstColEntity;
                    collisionHappened->m_secondColComponentType = collisionHappened->m_firstColComponentType;
                    collisionHappened->m_firstColEntity = tmp;
                    collisionHappened->m_firstColComponentType = tmpc;
                    componentsMap.at(collisionHappened->m_firstColEntity)->Receive(collisionHappened);
                }
            }
        }
        );
        movementEventHandler->RegisterHandler(
            MessageType::PLAYER_HIDDEN,
            [](MovementSystem* movementSystem, const std::unordered_map<EntityManager::Entity, MovementComponent*>& componentsMap, Event* event)
        {
            PlayerHidden* playerHidden = event->GetMessage<PlayerHidden>();
            componentsMap.at(playerHidden->m_playerEntity)->Receive(event);
        }
        );
        movementEventHandler->RegisterHandler(
            MessageType::CROUCH,
            [this](MovementSystem* movementSystem, const std::unordered_map<EntityManager::Entity, MovementComponent*>& componentsMap, Event* event)
        {
            componentsMap.at(event->m_entity)->Receive(event);
        }
        );

        MovementSystem* movementSystem = new MovementSystem(movementEventHandler);
        m_systems.push_back(movementSystem);
        m_systemsMap[SystemType::Movement] = movementSystem;
        std::function<MovementComponent*(MovementComponent*, EntityManager::Entity, const glm::dvec2&, DirectionType)> playerMovementConstructor =
            [](MovementComponent* movementComponent, EntityManager::Entity entity, const glm::dvec2& position, DirectionType direction)
        {
            if (!movementComponent)
            {
                components::PlayerMovementComponent* newComponent = new components::PlayerMovementComponent();
                newComponent->Create(entity, position, direction);
                movementComponent = newComponent;
            }
            else
            {
                dynamic_cast<components::PlayerMovementComponent*>(movementComponent)->Create(entity, position, direction);
            }
            return movementComponent;
        };
        movementSystem->RegisterComponent<EntityManager::Entity, const glm::dvec2&, DirectionType>(ComponentType::PlayerMovement, playerMovementConstructor);

        std::function<MovementComponent*(MovementComponent*, EntityManager::Entity, const glm::dvec2&, const glm::dvec2&, DirectionType, EntityManager*)> aiMovementConstructor =
            [](MovementComponent* movementComponent, EntityManager::Entity entity, const glm::dvec2& position, const glm::dvec2& playerPosition, DirectionType direction, EntityManager* entityManager)
        {
            if (!movementComponent)
            {
                components::AIMovementComponent* newComponent = new components::AIMovementComponent();
                newComponent->Create(entity, position, playerPosition, direction, entityManager);
                movementComponent = newComponent;
            }
            else
            {
                dynamic_cast<components::AIMovementComponent*>(movementComponent)->Create(entity, position, playerPosition, direction, entityManager);
            }
            return movementComponent;
        };
        movementSystem->RegisterComponent<EntityManager::Entity, const glm::dvec2&, const glm::dvec2&, DirectionType, EntityManager*>(ComponentType::AIMovement, aiMovementConstructor);

        std::function<MovementComponent*(MovementComponent*, EntityManager::Entity, const glm::dvec2&, DirectionType)> projectileMovementConstructor =
            [](MovementComponent* movementComponent, EntityManager::Entity entity, const glm::dvec2& position, DirectionType direction)
        {
            if (!movementComponent)
            {
                components::ProjectileMovementComponent* newComponent = new components::ProjectileMovementComponent();
                newComponent->Create(entity, position, direction);
                movementComponent = newComponent;
            }
            else
            {
                dynamic_cast<components::ProjectileMovementComponent*>(movementComponent)->Create(entity, position, direction);
            }
            return movementComponent;
        };
        movementSystem->RegisterComponent<EntityManager::Entity, const glm::dvec2&, DirectionType>(ComponentType::ProjectileMovement, projectileMovementConstructor);

        std::function<MovementComponent*(MovementComponent*, EntityManager::Entity, EntityManager::Entity, const glm::dvec2&, const glm::dvec2&, const std::vector<EntityManager::Entity>&)> elevatorMovementConstructor =
            [](MovementComponent* movementComponent, EntityManager::Entity entity, EntityManager::Entity playerEntity, const glm::dvec2& position, const glm::dvec2& playerPosition, const std::vector<EntityManager::Entity>& excludedEntities)
        {
            if (!movementComponent)
            {
                components::ElevatorMovementComponent* newComponent = new components::ElevatorMovementComponent();
                newComponent->Create(entity, playerEntity, position, playerPosition, excludedEntities);
                movementComponent = newComponent;
            }
            else
            {
                dynamic_cast<components::ElevatorMovementComponent*>(movementComponent)->Create(entity, playerEntity, position, playerPosition, excludedEntities);
            }
            return movementComponent;
        };
        movementSystem->RegisterComponent<EntityManager::Entity, EntityManager::Entity, const glm::dvec2&, const glm::dvec2&, const std::vector<EntityManager::Entity>&>(ComponentType::ElevatorMovement, elevatorMovementConstructor);

        movementSystem->Subscribe([movementSystem](Event* event) {movementSystem->Receive(event); });
        inputSystem->Subscribe([movementSystem](Event* event) {movementSystem->Receive(event); });
        movementSystem->Subscribe([this](Event* event) {Receive(event); });

        // collision system
        EventHandler<CollisionSystem, CollisionComponent>* collisionEventHandler = new EventHandler<CollisionSystem, CollisionComponent>();
        collisionEventHandler->RegisterHandler(
            MessageType::POSITION_CHANGE,
            [this](CollisionSystem* collisionSystem, const std::unordered_map<EntityManager::Entity, CollisionComponent*>& componentsMap, Event* event)
        {
            ChangePoisition* changePosition = event->GetMessage<ChangePoisition>();
            if (componentsMap.find(event->m_entity) != componentsMap.end())
            {
                componentsMap.at(event->m_entity)->SetCenterPosition(changePosition->m_position);
            }
        }
        );
        collisionEventHandler->RegisterHandler(
            MessageType::MOVE_ELEVATOR,
            [](CollisionSystem* collisionSystem, const std::unordered_map<EntityManager::Entity, CollisionComponent*>& componentsMap, Event* event)
        {
            MoveElevator* changePosition = event->GetMessage<MoveElevator>();
            for (auto entity : changePosition->m_elevatorEntities)
            {
                if (componentsMap.find(entity) != componentsMap.end())
                {
                    componentsMap.at(entity)->SetCenterPosition(glm::dvec2(0.0, changePosition->m_moveLength));
                }
            }
        }
        );
        collisionEventHandler->RegisterHandler(
            MessageType::MOVE_UP,
            [](CollisionSystem* collisionSystem, const std::unordered_map<EntityManager::Entity, CollisionComponent*>& componentsMap, Event* event)
        {
            MoveUp* moveUp = event->GetMessage<MoveUp>();
            for (auto const& component : componentsMap)
            {
                CollisionComponent* cc = component.second;
                bool excluded = false;
                for (auto excludedEntity : moveUp->m_excludedEntities)
                {
                    if (cc->GetEntity() == excludedEntity)
                    {
                        excluded = true;
                        break;
                    }
                }
                if (!excluded)
                {
                    cc->SetCenterPosition(glm::dvec2(0, -moveUp->m_moveLength));
                }
            }
        }
        );
        collisionEventHandler->RegisterHandler(
            MessageType::CREATE_PROJECTILE,
            [this](CollisionSystem* collisionSystem, const std::unordered_map<EntityManager::Entity, CollisionComponent*>& componentsMap, Event* event)
        {
            CreateProjectile* createProjectile = event->GetMessage<CreateProjectile>();
            CollisionComponent* cc = componentsMap.at(event->m_entity);
            int targetEntity = -1;
            if (event->m_entity != m_playerEntity)
            {
                targetEntity = m_playerEntity;
            }
            double xPosition;
            if (cc->GetPosition().x >= 630.0)
            {
                xPosition = 630.0;
            }
            else
            {
                xPosition = cc->GetPosition().x;
            }
            collisionSystem->CreateComponent(
                ComponentType::ProjectileCollision,
                createProjectile->m_newEntity,
                event->m_entity,
                targetEntity,
                glm::dvec2(xPosition, cc->GetPosition().y - 5),
                glm::ivec2(20, 5)
            );
        }
        );
        collisionEventHandler->RegisterHandler(
            MessageType::CREATE_ENEMY,
            [this](CollisionSystem* collisionSystem, const std::unordered_map<EntityManager::Entity, CollisionComponent*>& componentsMap, Event* event)
        {
            CreateEnemy* createEnemy = event->GetMessage<CreateEnemy>();
            collisionSystem->CreateComponent(
                ComponentType::CharacterCollision,
                createEnemy->m_newEntity,
                createEnemy->m_position,
                glm::ivec2(20, 40),
                false);
        }
        );

        collisionEventHandler->RegisterHandler(
            MessageType::PLAYER_HIDDEN,
            [](CollisionSystem* collisionSystem, const std::unordered_map<EntityManager::Entity, CollisionComponent*>& componentsMap, Event* event)
        {
            PlayerHidden* playerHidden = event->GetMessage<PlayerHidden>();
            componentsMap.at(playerHidden->m_playerEntity)->Receive(event);
        }
        );
        collisionEventHandler->RegisterHandler(
            MessageType::CROUCH,
            [this](CollisionSystem* collisionSystem, const std::unordered_map<EntityManager::Entity, CollisionComponent*>& componentsMap, Event* event)
        {
            componentsMap.at(event->m_entity)->Receive(event);
        }
        );

        CollisionSystem* collisionSystem = new CollisionSystem(collisionEventHandler);
        m_systems.push_back(collisionSystem);
        m_systemsMap[SystemType::Collision] = collisionSystem;

        std::function<CollisionComponent*(CollisionComponent*, EntityManager::Entity, EntityManager::Entity, int, const glm::dvec2&, const glm::ivec2&)> projectileCollisionConstructor =
            [](CollisionComponent* collisionComponent, EntityManager::Entity entity, EntityManager::Entity spawnedFromEntity, int targetEntity, const glm::dvec2& position, const glm::ivec2& size)
        {
            if (!collisionComponent)
            {
                components::ProjectileCollisionComponent* newComponent = new components::ProjectileCollisionComponent();
                newComponent->Create(entity, spawnedFromEntity, targetEntity, position, size);
                collisionComponent = newComponent;
            }
            else
            {
                dynamic_cast<components::ProjectileCollisionComponent*>(collisionComponent)->Create(entity, spawnedFromEntity, targetEntity, position, size);
            }
            return collisionComponent;
        };
        collisionSystem->RegisterComponent<EntityManager::Entity, EntityManager::Entity, int, const glm::dvec2&, const glm::ivec2&>(ComponentType::ProjectileCollision, projectileCollisionConstructor);

        std::function<CollisionComponent*(CollisionComponent*, EntityManager::Entity, const glm::dvec2&, const glm::ivec2&)> staticCollisionConstructor =
            [](CollisionComponent* collisionComponent, EntityManager::Entity entity, const glm::dvec2& position, const glm::ivec2& size)
        {
            if (!collisionComponent)
            {
                components::StaticCollisionComponent* newComponent = new components::StaticCollisionComponent();
                newComponent->Create(entity, position, size);
                collisionComponent = newComponent;
            }
            else
            {
                dynamic_cast<components::StaticCollisionComponent*>(collisionComponent)->Create(entity, position, size);
            }
            return collisionComponent;
        };
        collisionSystem->RegisterComponent<EntityManager::Entity, const glm::dvec2&, const glm::ivec2&>(ComponentType::StaticCollision, staticCollisionConstructor);

        std::function<CollisionComponent*(CollisionComponent*, EntityManager::Entity, const glm::dvec2&, const glm::ivec2&, bool)> characterCollisionConstructor =
            [](CollisionComponent* collisionComponent, EntityManager::Entity entity, const glm::dvec2& position, const glm::ivec2& size, bool isPlayerEntity)
        {
            if (!collisionComponent)
            {
                components::CharacterCollisionComponent* newComponent = new components::CharacterCollisionComponent();
                newComponent->Create(entity, position, size, isPlayerEntity);
                collisionComponent = newComponent;
            }
            else
            {
                dynamic_cast<components::CharacterCollisionComponent*>(collisionComponent)->Create(entity, position, size, isPlayerEntity);
            }
            return collisionComponent;
        };
        collisionSystem->RegisterComponent<EntityManager::Entity, const glm::dvec2&, const glm::ivec2&, bool>(ComponentType::CharacterCollision, characterCollisionConstructor);
        collisionSystem->Subscribe([movementSystem](Event* event) {movementSystem->Receive(event); });
        collisionSystem->Subscribe([this](Event* event) {Receive(event); });
        movementSystem->Subscribe([collisionSystem](Event* event) {collisionSystem->Receive(event); });
        inputSystem->Subscribe([collisionSystem](Event* event) {collisionSystem->Receive(event); });

        // camera system
        EventHandler<CameraSystem, CameraComponent>* cameraEventHandler = new EventHandler<CameraSystem, CameraComponent>();
        cameraEventHandler->RegisterHandler(
            MessageType::POSITION_CHANGE,
            [](CameraSystem* collisionSystem, const std::unordered_map<EntityManager::Entity, CameraComponent*>& componentsMap, Event* event)
        {
            ChangePoisition* changePosition = event->GetMessage<ChangePoisition>();
            if (componentsMap.find(event->m_entity) != componentsMap.end())
            {
                componentsMap.at(event->m_entity)->SetPosition(changePosition->m_position);
            }
        }
        );
        cameraEventHandler->RegisterHandler(
            MessageType::COLLISION_HAPPENED,
            [](CameraSystem* collisionSystem, const std::unordered_map<EntityManager::Entity, CameraComponent*>& componentsMap, Event* event)
        {
            CollisionHappened* collisionHappened = event->GetMessage<CollisionHappened>();

            if ((collisionHappened->m_firstColComponentType != ComponentType::StaticCollision) && (componentsMap.find(collisionHappened->m_firstColEntity) != componentsMap.end()))
            {
                componentsMap.at(collisionHappened->m_firstColEntity)->Receive(collisionHappened);
            }
            else if ((collisionHappened->m_secondColComponentType != ComponentType::StaticCollision) && (componentsMap.find(collisionHappened->m_secondColEntity) != componentsMap.end()))
            {
                // swap the order so the movement component will always treat itself as the first entity in the message
                EntityManager::Entity tmp = collisionHappened->m_secondColEntity;
                ComponentType tmpc = collisionHappened->m_secondColComponentType;
                DirectionType tmpd = collisionHappened->m_secondColDirection;
                collisionHappened->m_secondColEntity = collisionHappened->m_firstColEntity;
                collisionHappened->m_secondColComponentType = collisionHappened->m_firstColComponentType;
                collisionHappened->m_secondColDirection = collisionHappened->m_firstColDirection;
                collisionHappened->m_firstColEntity = tmp;
                collisionHappened->m_firstColComponentType = tmpc;
                collisionHappened->m_firstColDirection = tmpd;
                componentsMap.at(collisionHappened->m_firstColEntity)->Receive(collisionHappened);
            }
        }
        );

        CameraSystem* cameraSystem = new CameraSystem(cameraEventHandler);
        m_systems.push_back(cameraSystem);
        m_systemsMap[SystemType::Camera] = cameraSystem;

        std::function<CameraComponent*(CameraComponent*, EntityManager::Entity, const glm::dvec2&, const std::vector<EntityManager::Entity>&)> horizCameraConstr =
            [](CameraComponent* cameraComponent, EntityManager::Entity entity, const glm::dvec2& position, const std::vector<EntityManager::Entity>& excludedEntities) -> CameraComponent*
        {
            if (!cameraComponent)
            {
                components::HorizontalCameraComponent* newComponent = new components::HorizontalCameraComponent();
                newComponent->Create(entity, position, excludedEntities);
                cameraComponent = newComponent;
            }
            else
            {
                dynamic_cast<components::HorizontalCameraComponent*>(cameraComponent)->Create(entity, position, excludedEntities);
            }
            return cameraComponent;
        };
        cameraSystem->RegisterComponent<EntityManager::Entity, const glm::dvec2&, const std::vector<EntityManager::Entity>&>(ComponentType::HorizontalCamera, horizCameraConstr);
        movementSystem->Subscribe([cameraSystem](Event* event) {cameraSystem->Receive(event); });
        collisionSystem->Subscribe([cameraSystem](Event* event) {cameraSystem->Receive(event); });
        cameraSystem->Subscribe([movementSystem](Event* event) {movementSystem->Receive(event); });
        cameraSystem->Subscribe([collisionSystem](Event* event) {collisionSystem->Receive(event); });
        cameraSystem->Subscribe([this](Event* event) {Receive(event); });

        // render system
        EventHandler<RenderSystem, RenderComponent>* renderEventHandler = new EventHandler<RenderSystem, RenderComponent>();
        renderEventHandler->RegisterHandler(
            MessageType::POSITION_CHANGE,
            [this](RenderSystem* renderSystem, const std::unordered_map<EntityManager::Entity, RenderComponent*>& componentsMap, Event* event)
        {
            ChangePoisition* changePosition = event->GetMessage<ChangePoisition>();
            if (event->m_entity == m_playerEntity)
            {
                for (auto const& component : componentsMap)
                {
                    RenderComponent* rc = component.second;
                    if (event->m_entity == component.first)
                    {
                        rc->SetPosition(glm::dvec2(changePosition->m_position.x, 0.0));
                    }
                    else
                    {
                        rc->Receive(changePosition);
                    }
                }
            }
            else
            {
                componentsMap.at(event->m_entity)->SetPosition(changePosition->m_position);
            }
            componentsMap.at(event->m_entity)->Receive(event);
        }
        );
        renderEventHandler->RegisterHandler(
            MessageType::MOVE_ELEVATOR,
            [](RenderSystem* collisionSystem, const std::unordered_map<EntityManager::Entity, RenderComponent*>& componentsMap, Event* event)
        {
            MoveElevator* changePosition = event->GetMessage<MoveElevator>();
            for (auto entity : changePosition->m_elevatorEntities)
            {
                if (componentsMap.find(entity) != componentsMap.end())
                {
                    componentsMap.at(entity)->SetPosition(glm::dvec2(0.0, changePosition->m_moveLength));
                }
            }
        }
        );
        renderEventHandler->RegisterHandler(
            MessageType::MOVE_UP,
            [](RenderSystem* renderSystem, const std::unordered_map<EntityManager::Entity, RenderComponent*>& componentsMap, Event* event)
        {
            MoveUp* moveUp = event->GetMessage<MoveUp>();
            for (auto const& component : componentsMap)
            {
                RenderComponent* rc = component.second;
                bool excluded = false;
                for (auto excludedEntity : moveUp->m_excludedEntities)
                {
                    if (rc->GetEntity() == excludedEntity)
                    {
                        excluded = true;
                        break;
                    }
                }
                if (!excluded)
                {
                    rc->SetPosition(glm::dvec2(0, -moveUp->m_moveLength));
                }
            }
        }
        );
        renderEventHandler->RegisterHandler(
            MessageType::CREATE_PROJECTILE,
            [this](RenderSystem* renderSystem, const std::unordered_map<EntityManager::Entity, RenderComponent*>& componentsMap, Event* event)
        {
            CreateProjectile* createProjectile = event->GetMessage<CreateProjectile>();
            RenderComponent* rc = componentsMap.at(event->m_entity);
            MovementType mt = rc->GetMovementType();
            if ((mt == MovementType::NO_MOVEMENT_LEFT) || (mt == MovementType::CROUCHING_LEFT))
            {
                mt = MovementType::LEFT;
            }
            else if ((mt == MovementType::NO_MOVEMENT_RIGHT) || (mt == MovementType::CROUCHING_RIGHT))
            {
                mt = RIGHT;
            }
            double posOffset = 0;
            if ((rc->GetMovementType() == MovementType::CROUCHING_LEFT) || (rc->GetMovementType() == MovementType::CROUCHING_RIGHT))
            {
                posOffset = 25;
            }
            else
            {
                posOffset = 15;
            }
            renderSystem->CreateComponent(
                ComponentType::SpriteRender,
                createProjectile->m_newEntity,
                m_renderer,
                std::string(createProjectile->m_projetileSprites),
                glm::dvec2(rc->GetSpritePosition().x, rc->GetSpritePosition().y + posOffset),
                mt);
        }
        );
        renderEventHandler->RegisterHandler(
            MessageType::CREATE_ENEMY,
            [this](RenderSystem* renderSystem, const std::unordered_map<EntityManager::Entity, RenderComponent*>& componentsMap, Event* event)
        {
            CreateEnemy* createEnemy = event->GetMessage<CreateEnemy>();
            MovementType mt;
            if (createEnemy->m_position.x < 370.0)
            {
                mt = MovementType::NO_MOVEMENT_RIGHT;
            }
            else
            {
                mt = MovementType::NO_MOVEMENT_LEFT;
            }
            renderSystem->CreateComponent(
                ComponentType::SpritesheetRender,
                createEnemy->m_newEntity,
                m_renderer,
                std::string("data/sprite/enemy/enemy.json"),
                createEnemy->m_position,
                mt);
        }
        );

        renderEventHandler->RegisterHandler(
            MessageType::COLLISION_HAPPENED,
            [this](RenderSystem* renderSystem, const std::unordered_map<EntityManager::Entity, RenderComponent*>& componentsMap, Event* event)
        {
            CollisionHappened* collisionHappened = event->GetMessage<CollisionHappened>();
            if (collisionHappened->m_firstColComponentType != ComponentType::StaticCollision)
            {
                if (!((collisionHappened->m_firstColEntity == m_playerEntity) && (collisionHappened->m_firstColComponentType == ComponentType::CharacterCollision) &&
                    (collisionHappened->m_secondColComponentType == ComponentType::ProjectileCollision)))
                {
                    componentsMap.at(collisionHappened->m_firstColEntity)->Receive(collisionHappened);
                }
            }
            if ((collisionHappened->m_secondColComponentType != ComponentType::StaticCollision) && (collisionHappened->m_secondColEntity != m_playerEntity))
            {
                if (!((collisionHappened->m_secondColEntity == m_playerEntity) && (collisionHappened->m_secondColComponentType == ComponentType::CharacterCollision) &&
                    (collisionHappened->m_firstColComponentType == ComponentType::ProjectileCollision)))
                {
                    // swap the order so the movement component will always treat itself as the first entity in the message
                    EntityManager::Entity tmp = collisionHappened->m_secondColEntity;
                    ComponentType tmpc = collisionHappened->m_secondColComponentType;
                    DirectionType tmpd = collisionHappened->m_secondColDirection;
                    collisionHappened->m_secondColEntity = collisionHappened->m_firstColEntity;
                    collisionHappened->m_secondColComponentType = collisionHappened->m_firstColComponentType;
                    collisionHappened->m_secondColDirection = collisionHappened->m_firstColDirection;
                    collisionHappened->m_firstColEntity = tmp;
                    collisionHappened->m_firstColComponentType = tmpc;
                    collisionHappened->m_firstColDirection = tmpd;
                    componentsMap.at(collisionHappened->m_firstColEntity)->Receive(collisionHappened);
                }
            }
        }
        );
        renderEventHandler->RegisterHandler(
            MessageType::PLAYER_ELIMINATED,
            [](RenderSystem* renderSystem, const std::unordered_map<EntityManager::Entity, RenderComponent*>& componentsMap, Event* event)
        {
            if (componentsMap.find(event->m_entity) != componentsMap.end())
            {
                componentsMap.at(event->m_entity)->Receive(event);
            }
        }
        );
        renderEventHandler->RegisterHandler(
            MessageType::PLAYER_FINISHED,
            [](RenderSystem* renderSystem, const std::unordered_map<EntityManager::Entity, RenderComponent*>& componentsMap, Event* event)
        {
            componentsMap.at(event->m_entity)->Receive(event);
        }
        );
        renderEventHandler->RegisterHandler(
            MessageType::DRIVE_CAR,
            [](RenderSystem* renderSystem, const std::unordered_map<EntityManager::Entity, RenderComponent*>& componentsMap, Event* event)
        {
            if (componentsMap.find(event->m_entity) != componentsMap.end())
            {
                componentsMap.at(event->m_entity)->Receive(event);
            }
        }
        );
        renderEventHandler->RegisterHandler(
            MessageType::OPEN_DOOR,
            [](RenderSystem* renderSystem, const std::unordered_map<EntityManager::Entity, RenderComponent*>& componentsMap, Event* event)
        {
            componentsMap.at(event->m_entity)->Receive(event);
        }
        );
        renderEventHandler->RegisterHandler(
            MessageType::PLAYER_HIDDEN,
            [](RenderSystem* renderSystem, const std::unordered_map<EntityManager::Entity, RenderComponent*>& componentsMap, Event* event)
        {
            PlayerHidden* playerHidden = event->GetMessage<PlayerHidden>();
            componentsMap.at(playerHidden->m_playerEntity)->Receive(event);
        }
        );
        renderEventHandler->RegisterHandler(
            MessageType::SPAWN_ENEMY,
            [this](RenderSystem* renderSystem, const std::unordered_map<EntityManager::Entity, RenderComponent*>& componentsMap, Event* event)
        {
            if (event->m_entity == m_playerEntity)
            {
                for (const auto& component : componentsMap)
                {
                    component.second->Receive(event);
                }
            }
        }
        );

        RenderSystem* renderSystem = new RenderSystem(renderEventHandler);
        m_systems.push_back(renderSystem);
        m_systemsMap[SystemType::Render] = renderSystem;

        std::function<RenderComponent*(RenderComponent*, EntityManager::Entity, SDL_Renderer*, const std::string&, const glm::dvec2&, MovementType)> spritesheetRenderConstr =
            [](RenderComponent* renderComponent, EntityManager::Entity entity, SDL_Renderer* renderer, const std::string& path, const glm::dvec2& position, MovementType movementType) -> RenderComponent*
        {
            if (!renderComponent)
            {
                components::SpritesheetRenderComponent* newComponent = new components::SpritesheetRenderComponent();
                newComponent->Create(entity, renderer, path, position, movementType);
                renderComponent = newComponent;
            }
            else
            {
                dynamic_cast<components::SpritesheetRenderComponent*>(renderComponent)->Create(entity, renderer, path, position, movementType);
            }
            return renderComponent;
        };
        renderSystem->RegisterComponent<EntityManager::Entity, SDL_Renderer*, const std::string&, const glm::dvec2&, MovementType>(
            ComponentType::SpritesheetRender, spritesheetRenderConstr);

        std::function<RenderComponent*(RenderComponent*, EntityManager::Entity, SDL_Renderer*, const std::string&, const glm::dvec2&, MovementType)> spriteRenderConstr =
            [](RenderComponent* renderComponent, EntityManager::Entity entity, SDL_Renderer* renderer, const std::string& path, const glm::dvec2& position, MovementType movementType) -> RenderComponent*
        {
            if (!renderComponent)
            {
                components::SpriteRenderComponent* newComponent = new components::SpriteRenderComponent();
                newComponent->Create(entity, renderer, path, position, movementType);
                renderComponent = newComponent;
            }
            else
            {
                dynamic_cast<components::SpriteRenderComponent*>(renderComponent)->Create(entity, renderer, path, position, movementType);
            }
            return renderComponent;
        };
        renderSystem->RegisterComponent<EntityManager::Entity, SDL_Renderer*, const std::string&, const glm::dvec2&, MovementType>(
            ComponentType::SpriteRender, spriteRenderConstr);

        std::function<RenderComponent*(RenderComponent*, EntityManager::Entity, SDL_Renderer*, const std::string&, const glm::dvec2&, const glm::ivec2&)> staticRenderConstr =
            [](RenderComponent* renderComponent, EntityManager::Entity entity, SDL_Renderer* renderer, const std::string& path, const glm::dvec2& position, const glm::ivec2& size) -> RenderComponent*
        {
            if (!renderComponent)
            {
                components::StaticRenderComponent* newComponent = new components::StaticRenderComponent();
                newComponent->Create(entity, renderer, path, position, size);
                renderComponent = newComponent;
            }
            else
            {
                dynamic_cast<components::StaticRenderComponent*>(renderComponent)->Create(entity, renderer, path, position, size);
            }
            return renderComponent;
        };
        renderSystem->RegisterComponent<EntityManager::Entity, SDL_Renderer*, const std::string&, const glm::dvec2&, const glm::ivec2&>(
            ComponentType::StaticRender, staticRenderConstr);

        std::function<RenderComponent*(RenderComponent*, EntityManager::Entity, SDL_Renderer*, const std::string&, const glm::dvec2&, const glm::dvec2&)> doorRenderConstr =
            [this](RenderComponent* renderComponent, EntityManager::Entity entity, SDL_Renderer* renderer, const std::string& path, const glm::dvec2& position, const glm::dvec2& playerPosition) -> RenderComponent*
        {
            if (!renderComponent)
            {
                components::DoorRenderComponent* newComponent = new components::DoorRenderComponent();
                newComponent->Create(entity, m_playerEntity, renderer, path, position, playerPosition);
                renderComponent = newComponent;
            }
            else
            {
                dynamic_cast<components::DoorRenderComponent*>(renderComponent)->Create(entity, m_playerEntity, renderer, path, position, playerPosition);
            }
            return renderComponent;
        };
        renderSystem->RegisterComponent<EntityManager::Entity, SDL_Renderer*, const std::string&, const glm::dvec2&, const glm::dvec2&>(
            ComponentType::DoorRender, doorRenderConstr);

        std::function<RenderComponent*(RenderComponent*, EntityManager::Entity, SDL_Renderer*, const std::string&, const glm::dvec2&)> enemyDoorRenderConstr =
            [this](RenderComponent* renderComponent, EntityManager::Entity entity, SDL_Renderer* renderer, const std::string& path, const glm::dvec2& position) -> RenderComponent*
        {
            if (!renderComponent)
            {
                components::EnemyDoorRenderComponent* newComponent = new components::EnemyDoorRenderComponent();
                newComponent->Create(entity, renderer, path, position, m_entityManager);
                renderComponent = newComponent;
            }
            else
            {
                dynamic_cast<components::EnemyDoorRenderComponent*>(renderComponent)->Create(entity, renderer, path, position, m_entityManager);
            }
            return renderComponent;
        };
        renderSystem->RegisterComponent< EntityManager::Entity, SDL_Renderer*, const std::string&, const glm::dvec2&>(
            ComponentType::EnemyDoorRender, enemyDoorRenderConstr);

        movementSystem->Subscribe([renderSystem](Event* event) {renderSystem->Receive(event); });
        inputSystem->Subscribe([renderSystem](Event* event) {renderSystem->Receive(event); });
        collisionSystem->Subscribe([renderSystem](Event* event) {renderSystem->Receive(event); });
        cameraSystem->Subscribe([renderSystem](Event* event) {renderSystem->Receive(event); });
        Subscribe([renderSystem](Event* event) {renderSystem->Receive(event); });
        renderSystem->Subscribe([this](Event* event) {Receive(event); });
        renderSystem->Subscribe([movementSystem](Event* event) {movementSystem->Receive(event); });
        renderSystem->Subscribe([collisionSystem](Event* event) {collisionSystem->Receive(event); });
        renderSystem->Subscribe([renderSystem](Event* event) {renderSystem->Receive(event); });

        m_entityManager = new EntityManager();
        m_playerLives = 3;
        m_playerHorizontalPosition = 0;
        CreateLevel(renderSystem, collisionSystem, movementSystem, inputSystem, cameraSystem);

        // game input handling
        m_gameEntity = m_entityManager->CreateEntity();
        InputHandler* inputHandler = new InputHandler();
        inputHandler->RegisterHandler(
            SDLK_ESCAPE,
            [](InputComponent* inputComponent, std::unordered_map<SDL_Keycode, bool>& keycodes, Uint32 type)
        {
            ExitGame* message = nullptr;
            if (type == SDL_KEYDOWN)
            {
                Event* event = EventPool::GetEvent(EXIT_GAME);
                if (event)
                {
                    message = event->GetMessage<ExitGame>();
                }
                else
                {
                    message = new ExitGame();
                }
                message->m_type = MessageType::EXIT_GAME;
            }
            return message;
        }
        );
        std::vector<SDL_Keycode> keycodes{ SDLK_ESCAPE };
        inputSystem->CreateComponent(ComponentType::BasicInput, m_gameEntity, inputHandler, keycodes);

        m_hudTexture = RenderManager::CreateTexture(m_renderer, "data/sprite/hud/hud_bckg.png");
        m_hudLives = RenderManager::CreateTexture(m_renderer, "data/sprite/hud/hud_live.png");
        m_backgroundMusic = SoundManager::LoadBackGroundMusic("data/sounds/theme.mp3");
        m_shoot = SoundManager::LoadSFXMusic("data/sounds/shoot.wav");
        m_hit = SoundManager::LoadSFXMusic("data/sounds/hit.wav");
        m_failed = SoundManager::LoadSFXMusic("data/sounds/failed.wav");
        m_car = SoundManager::LoadSFXMusic("data/sounds/car.wav");
        SoundManager::PlayBackGroundMusic(m_backgroundMusic);
    }

    void GameSystem::CreateLevel(RenderSystem* renderSystem, CollisionSystem* collisionSystem, MovementSystem* movementSystem, InputSystem* inputSystem, CameraSystem* cameraSystem)
    {
        m_gameStart = true;
        m_levelOver = false;
        m_levelFinished = false;
        m_drawScore = false;
        m_playerTemporaryDisabled = false;
        m_bonusScore = false;
        m_buildingLevel = 0;
        m_playerScore = 0;
        m_playerEntity = m_entityManager->CreateEntity();
        double offset = 0.0;
        for (int i = 0; i < 31; i++, offset += 95.0)
        {
            EntityManager::Entity wallEntity = m_entityManager->CreateEntity();
            renderSystem->CreateComponent(ComponentType::StaticRender, wallEntity, m_renderer, std::string("data/sprite/building/wall_vertical.png"), glm::dvec2(140.0, 50.0 + offset), glm::ivec2(10, 105));
            collisionSystem->CreateComponent(ComponentType::StaticCollision, wallEntity, glm::dvec2(140.0, 50.0 + offset), glm::ivec2(10, 105));
            if (i != 30)
            {
                wallEntity = m_entityManager->CreateEntity();
                renderSystem->CreateComponent(ComponentType::StaticRender, wallEntity, m_renderer, std::string("data/sprite/building/wall_vertical.png"), glm::dvec2(650.0, 50.0 + offset), glm::ivec2(10, 105));
                collisionSystem->CreateComponent(ComponentType::StaticCollision, wallEntity, glm::dvec2(650.0, 50.0 + offset), glm::ivec2(10, 105));
            }

            wallEntity = m_entityManager->CreateEntity();
            renderSystem->CreateComponent(ComponentType::StaticRender, wallEntity, m_renderer, std::string("data/sprite/building/wall_horizontal.png"), glm::dvec2(150.0, 50.0 + offset), glm::ivec2(225, 10));
            collisionSystem->CreateComponent(ComponentType::StaticCollision, wallEntity, glm::dvec2(150.0, 50.0 + offset), glm::ivec2(225, 10));
            wallEntity = m_entityManager->CreateEntity();
            renderSystem->CreateComponent(ComponentType::StaticRender, wallEntity, m_renderer, std::string("data/sprite/building/wall_horizontal.png"), glm::dvec2(425.0, 50.0 + offset), glm::ivec2(225, 10));
            collisionSystem->CreateComponent(ComponentType::StaticCollision, wallEntity, glm::dvec2(425.0, 50.0 + offset), glm::ivec2(225, 10));

            wallEntity = m_entityManager->CreateEntity();
            renderSystem->CreateComponent(ComponentType::StaticRender, wallEntity, m_renderer, std::string("data/sprite/building/wall.png"), glm::dvec2(150.0, 60.0 + offset), glm::ivec2(220, 85));
            wallEntity = m_entityManager->CreateEntity();
            renderSystem->CreateComponent(ComponentType::StaticRender, wallEntity, m_renderer, std::string("data/sprite/building/wall.png"), glm::dvec2(430.0, 60.0 + offset), glm::ivec2(220, 85));
            wallEntity = m_entityManager->CreateEntity();
            renderSystem->CreateComponent(ComponentType::StaticRender, wallEntity, m_renderer, std::string("data/sprite/building/lamp.png"), glm::dvec2(260.0, 60.0 + offset), glm::ivec2(10, 15));
            wallEntity = m_entityManager->CreateEntity();
            renderSystem->CreateComponent(ComponentType::StaticRender, wallEntity, m_renderer, std::string("data/sprite/building/lamp.png"), glm::dvec2(540.0, 60.0 + offset), glm::ivec2(10, 15));

            double numberOffset = 0.0;
            for (const auto& levelNumberSprite : m_levelNumbers.GetLevelNumbers(30 - i))
            {
                renderSystem->CreateComponent(ComponentType::StaticRender, m_entityManager->CreateEntity(), m_renderer, levelNumberSprite, glm::dvec2(600.0 + numberOffset, 60.0 + offset), glm::ivec2(15, 15));
                numberOffset += 15.0;
            }

            if (((i % 2) == 0) && (i != 0) && (i != 30))
            {
                wallEntity = m_entityManager->CreateEntity();
                InputHandler* doorInputHandler = new InputHandler();
                doorInputHandler->RegisterHandler(
                    SDLK_UP,
                    [](InputComponent* inputComponent, std::unordered_map<SDL_Keycode, bool>& keycodes, Uint32 type)
                {
                    OpenDoor* message = nullptr;
                    if (type == SDL_KEYDOWN)
                    {
                        Event* event = EventPool::GetEvent(OPEN_DOOR);
                        if (event)
                        {
                            message = event->GetMessage<OpenDoor>();
                        }
                        else
                        {
                            message = new OpenDoor();
                        }
                        message = new OpenDoor();
                        message->m_type = MessageType::OPEN_DOOR;
                    }
                    return message;
                }
                );
                std::vector<SDL_Keycode> keycodes{ SDLK_UP };
                inputSystem->CreateComponent(ComponentType::BasicInput, wallEntity, doorInputHandler, keycodes);
                int random = RandomNumberGenerator::GenerateRandomNumber(0, 3);
                if (random == 0)
                {
                    renderSystem->CreateComponent(ComponentType::DoorRender, wallEntity, m_renderer, std::string("data/sprite/door/left/door.json"), glm::dvec2(200.0, 85.0 + offset), glm::dvec2(160.0, 105.0));
                    wallEntity = m_entityManager->CreateEntity();
                    renderSystem->CreateComponent(ComponentType::EnemyDoorRender, wallEntity, m_renderer, std::string("data/sprite/enemydoor/left/door.json"), glm::dvec2(300.0, 85.0 + offset));
                    wallEntity = m_entityManager->CreateEntity();
                    renderSystem->CreateComponent(ComponentType::EnemyDoorRender, wallEntity, m_renderer, std::string("data/sprite/enemydoor/right/door.json"), glm::dvec2(480.0, 85.0 + offset));
                    wallEntity = m_entityManager->CreateEntity();
                    renderSystem->CreateComponent(ComponentType::EnemyDoorRender, wallEntity, m_renderer, std::string("data/sprite/enemydoor/right/door.json"), glm::dvec2(580.0, 85.0 + offset));
                }
                if (random == 1)
                {
                    renderSystem->CreateComponent(ComponentType::DoorRender, wallEntity, m_renderer, std::string("data/sprite/door/left/door.json"), glm::dvec2(300.0, 85.0 + offset), glm::dvec2(160.0, 105.0));
                    wallEntity = m_entityManager->CreateEntity();
                    renderSystem->CreateComponent(ComponentType::EnemyDoorRender, wallEntity, m_renderer, std::string("data/sprite/enemydoor/left/door.json"), glm::dvec2(200.0, 85.0 + offset));
                    wallEntity = m_entityManager->CreateEntity();
                    renderSystem->CreateComponent(ComponentType::EnemyDoorRender, wallEntity, m_renderer, std::string("data/sprite/enemydoor/right/door.json"), glm::dvec2(480.0, 85.0 + offset));
                    wallEntity = m_entityManager->CreateEntity();
                    renderSystem->CreateComponent(ComponentType::EnemyDoorRender, wallEntity, m_renderer, std::string("data/sprite/enemydoor/right/door.json"), glm::dvec2(580.0, 85.0 + offset));
                }
                if (random == 2)
                {
                    renderSystem->CreateComponent(ComponentType::DoorRender, wallEntity, m_renderer, std::string("data/sprite/door/right/door.json"), glm::dvec2(480.0, 85.0 + offset), glm::dvec2(160.0, 105.0));
                    wallEntity = m_entityManager->CreateEntity();
                    renderSystem->CreateComponent(ComponentType::EnemyDoorRender, wallEntity, m_renderer, std::string("data/sprite/enemydoor/left/door.json"), glm::dvec2(200.0, 85.0 + offset));
                    wallEntity = m_entityManager->CreateEntity();
                    renderSystem->CreateComponent(ComponentType::EnemyDoorRender, wallEntity, m_renderer, std::string("data/sprite/enemydoor/left/door.json"), glm::dvec2(300.0, 85.0 + offset));
                    wallEntity = m_entityManager->CreateEntity();
                    renderSystem->CreateComponent(ComponentType::EnemyDoorRender, wallEntity, m_renderer, std::string("data/sprite/enemydoor/right/door.json"), glm::dvec2(580.0, 85.0 + offset));
                }
                if (random == 3)
                {
                    renderSystem->CreateComponent(ComponentType::DoorRender, wallEntity, m_renderer, std::string("data/sprite/door/right/door.json"), glm::dvec2(580.0, 85.0 + offset), glm::dvec2(160.0, 105.0));
                    wallEntity = m_entityManager->CreateEntity();
                    renderSystem->CreateComponent(ComponentType::EnemyDoorRender, wallEntity, m_renderer, std::string("data/sprite/enemydoor/left/door.json"), glm::dvec2(200.0, 85.0 + offset));
                    wallEntity = m_entityManager->CreateEntity();
                    renderSystem->CreateComponent(ComponentType::EnemyDoorRender, wallEntity, m_renderer, std::string("data/sprite/enemydoor/left/door.json"), glm::dvec2(300.0, 85.0 + offset));
                    wallEntity = m_entityManager->CreateEntity();
                    renderSystem->CreateComponent(ComponentType::EnemyDoorRender, wallEntity, m_renderer, std::string("data/sprite/enemydoor/right/door.json"), glm::dvec2(480.0, 85.0 + offset));
                }
            }
            else if ((i != 0) && (i != 30))
            {
                wallEntity = m_entityManager->CreateEntity();
                renderSystem->CreateComponent(ComponentType::EnemyDoorRender, wallEntity, m_renderer, std::string("data/sprite/enemydoor/left/door.json"), glm::dvec2(200.0, 85.0 + offset));
                wallEntity = m_entityManager->CreateEntity();
                renderSystem->CreateComponent(ComponentType::EnemyDoorRender, wallEntity, m_renderer, std::string("data/sprite/enemydoor/left/door.json"), glm::dvec2(300.0, 85.0 + offset));
                wallEntity = m_entityManager->CreateEntity();
                renderSystem->CreateComponent(ComponentType::EnemyDoorRender, wallEntity, m_renderer, std::string("data/sprite/enemydoor/right/door.json"), glm::dvec2(480.0, 85.0 + offset));
                wallEntity = m_entityManager->CreateEntity();
                renderSystem->CreateComponent(ComponentType::EnemyDoorRender, wallEntity, m_renderer, std::string("data/sprite/enemydoor/right/door.json"), glm::dvec2(580.0, 85.0 + offset));
            }

            wallEntity = m_entityManager->CreateEntity();
            renderSystem->CreateComponent(ComponentType::StaticRender, wallEntity, m_renderer, std::string("data/sprite/building/wall_horizontal.png"), glm::dvec2(150.0, 145.0 + offset), glm::ivec2(225, 10));
            collisionSystem->CreateComponent(ComponentType::StaticCollision, wallEntity, glm::dvec2(150.0, 145.0 + offset), glm::ivec2(225, 10));
            wallEntity = m_entityManager->CreateEntity();
            renderSystem->CreateComponent(ComponentType::StaticRender, wallEntity, m_renderer, std::string("data/sprite/building/floor.png"), glm::dvec2(200.0, 135.0 + offset), glm::ivec2(140, 10));
            wallEntity = m_entityManager->CreateEntity();
            renderSystem->CreateComponent(ComponentType::StaticRender, wallEntity, m_renderer, std::string("data/sprite/building/floor_edge.png"), glm::dvec2(340.0, 135.0 + offset), glm::ivec2(35, 10));
            wallEntity = m_entityManager->CreateEntity();
            renderSystem->CreateComponent(ComponentType::StaticRender, wallEntity, m_renderer, std::string("data/sprite/building/wall_corner.png"), glm::dvec2(150.0, 60.0 + offset), glm::ivec2(10, 85));
            wallEntity = m_entityManager->CreateEntity();
            renderSystem->CreateComponent(ComponentType::StaticRender, wallEntity, m_renderer, std::string("data/sprite/building/floor_corner_inverted.png"), glm::dvec2(150.0, 135.0 + offset), glm::ivec2(50, 10));

            if (i == 30)
            {
                wallEntity = m_entityManager->CreateEntity();
                renderSystem->CreateComponent(ComponentType::StaticRender, wallEntity, m_renderer, std::string("data/sprite/building/wall_horizontal.png"), glm::dvec2(425.0, 145.0 + offset), glm::ivec2(375, 10));
                collisionSystem->CreateComponent(ComponentType::StaticCollision, wallEntity, glm::dvec2(425.0, 145.0 + offset), glm::ivec2(375, 10));
                wallEntity = m_entityManager->CreateEntity();
                renderSystem->CreateComponent(ComponentType::StaticRender, wallEntity, m_renderer, std::string("data/sprite/building/floor_edge_inverted.png"), glm::dvec2(425.0, 135.0 + offset), glm::ivec2(35, 10));
                wallEntity = m_entityManager->CreateEntity();
                renderSystem->CreateComponent(ComponentType::StaticRender, wallEntity, m_renderer, std::string("data/sprite/building/floor.png"), glm::dvec2(460.0, 135.0 + offset), glm::ivec2(340, 10));
            }
            else
            {
                wallEntity = m_entityManager->CreateEntity();
                renderSystem->CreateComponent(ComponentType::StaticRender, wallEntity, m_renderer, std::string("data/sprite/building/wall_horizontal.png"), glm::dvec2(425.0, 145.0 + offset), glm::ivec2(225, 10));
                collisionSystem->CreateComponent(ComponentType::StaticCollision, wallEntity, glm::dvec2(425.0, 145.0 + offset), glm::ivec2(225, 10));
                wallEntity = m_entityManager->CreateEntity();
                renderSystem->CreateComponent(ComponentType::StaticRender, wallEntity, m_renderer, std::string("data/sprite/building/wall_corner.png"), glm::dvec2(640.0, 60.0 + offset), glm::ivec2(10, 85));
                wallEntity = m_entityManager->CreateEntity();
                renderSystem->CreateComponent(ComponentType::StaticRender, wallEntity, m_renderer, std::string("data/sprite/building/floor_edge_inverted.png"), glm::dvec2(425.0, 135.0 + offset), glm::ivec2(35, 10));
                wallEntity = m_entityManager->CreateEntity();
                renderSystem->CreateComponent(ComponentType::StaticRender, wallEntity, m_renderer, std::string("data/sprite/building/floor.png"), glm::dvec2(460.0, 135.0 + offset), glm::ivec2(140, 10));
                wallEntity = m_entityManager->CreateEntity();
                renderSystem->CreateComponent(ComponentType::StaticRender, wallEntity, m_renderer, std::string("data/sprite/building/floor_corner.png"), glm::dvec2(600.0, 135.0 + offset), glm::ivec2(50, 10));
            }

            if (i == 30)
            {
                m_carEntity = m_entityManager->CreateEntity();
                renderSystem->CreateComponent(
                    ComponentType::SpriteRender,
                    m_carEntity,
                    m_renderer,
                    std::string("data/sprite/car/car.json"),
                    glm::dvec2(160.0, 105.0 + offset),
                    MovementType::RIGHT);
            }
        }

        EntityManager::Entity elevatorEntity1 = m_entityManager->CreateEntity();
        EntityManager::Entity elevatorEntity2 = m_entityManager->CreateEntity();
        EntityManager::Entity elevatorEntity3 = m_entityManager->CreateEntity();

        renderSystem->CreateComponent(ComponentType::StaticRender, elevatorEntity1, m_renderer, std::string("data/sprite/building/elevator_background.png"), glm::dvec2(370.0, 60.0), glm::ivec2(60, 75));
        renderSystem->CreateComponent(ComponentType::StaticRender, elevatorEntity2, m_renderer, std::string("data/sprite/building/elevator.png"), glm::dvec2(370.0, 40.0), glm::ivec2(60, 20));
        renderSystem->CreateComponent(ComponentType::StaticRender, elevatorEntity3, m_renderer, std::string("data/sprite/building/elevator.png"), glm::dvec2(370.0, 135.0), glm::ivec2(60, 20));
        movementSystem->CreateComponent(ComponentType::ElevatorMovement, elevatorEntity3, m_playerEntity, glm::dvec2(370.0, 135.0), glm::dvec2(160.0, 95.0), std::vector<EntityManager::Entity>{m_playerEntity, elevatorEntity1, elevatorEntity2, elevatorEntity3});
        collisionSystem->CreateComponent(ComponentType::StaticCollision, elevatorEntity3, glm::dvec2(375.0, 145.0), glm::ivec2(50, 10));

        InputHandler* inputHandler = new InputHandler();
        inputHandler->RegisterHandler(
            SDLK_DOWN,
            [](InputComponent* inputComponent, std::unordered_map<SDL_Keycode, bool>& keycodes, Uint32 type)
        {
            Move* message = nullptr;
            Event* event = EventPool::GetEvent(MOVE);
            if (event)
            {
                message = event->GetMessage<Move>();
            }
            else
            {
                message = new Move();
            }
            message->m_type = MessageType::MOVE;
            if (type == SDL_KEYDOWN)
            {
                message->m_movement = MovementType::DOWN;
            }
            else
            {
                message->m_movement = MovementType::NO_MOVEMENT;
            }
            return message;
        }
        );
        std::vector<SDL_Keycode> keycodes{ SDLK_DOWN };
        inputSystem->CreateComponent(ComponentType::BasicInput, elevatorEntity3, inputHandler, keycodes);

        // player entity
        movementSystem->CreateComponent(ComponentType::PlayerMovement, m_playerEntity, glm::dvec2(160.0, 105.0), DirectionType::DIR_RIGHT);
        renderSystem->CreateComponent(ComponentType::SpritesheetRender, m_playerEntity, m_renderer, std::string("data/sprite/player/player.json"), glm::dvec2(160.0, 95.0), MovementType::NO_MOVEMENT_RIGHT);
        collisionSystem->CreateComponent(ComponentType::CharacterCollision, m_playerEntity, glm::dvec2(160.0, 105.0), glm::ivec2(20, 40), true);
        cameraSystem->CreateComponent(ComponentType::HorizontalCamera, m_playerEntity, glm::dvec2(160.0, 105.0), std::vector<EntityManager::Entity>{m_playerEntity});
        m_playerPosition = glm::dvec2(160.0, 105.0);
        m_playerLastPosition = m_playerPosition;

        // create input handler for player input component
        inputHandler = new InputHandler();
        inputHandler->RegisterHandler(
            SDLK_LEFT,
            [](InputComponent* inputComponent, std::unordered_map<SDL_Keycode, bool>& keycodes, Uint32 type)
        {
            Move* message = nullptr;
            if (type == SDL_KEYDOWN)
            {
                keycodes[SDLK_LEFT] = true;
                Event* event = EventPool::GetEvent(MOVE);
                if (event)
                {
                    message = event->GetMessage<Move>();
                }
                else
                {
                    message = new Move();
                }
                message->m_movement = MovementType::LEFT;
                message->m_type = MessageType::MOVE;
            }
            else
            {
                keycodes[SDLK_LEFT] = false;
                if (!keycodes[SDLK_RIGHT] && (!keycodes[SDLK_LCTRL]))
                {
                    Event* event = EventPool::GetEvent(MOVE);
                    if (event)
                    {
                        message = event->GetMessage<Move>();
                    }
                    else
                    {
                        message = new Move();
                    }
                    message->m_type = MessageType::MOVE;
                    message->m_movement = MovementType::NO_MOVEMENT;
                }
            }
            return message;
        }
        );
        inputHandler->RegisterHandler(
            SDLK_RIGHT,
            [](InputComponent* inputComponent, std::unordered_map<SDL_Keycode, bool>& keycodes, Uint32 type)
        {
            Move* message = nullptr;
            if (type == SDL_KEYDOWN)
            {
                keycodes[SDLK_RIGHT] = true;
                Event* event = EventPool::GetEvent(MOVE);
                if (event)
                {
                    message = event->GetMessage<Move>();
                }
                else
                {
                    message = new Move();
                }
                message->m_movement = MovementType::RIGHT;
                message->m_type = MessageType::MOVE;
            }
            else
            {
                keycodes[SDLK_RIGHT] = false;
                if (!keycodes[SDLK_LEFT] && (!keycodes[SDLK_LCTRL]))
                {
                    Event* event = EventPool::GetEvent(MOVE);
                    if (event)
                    {
                        message = event->GetMessage<Move>();
                    }
                    else
                    {
                        message = new Move();
                    }
                    message->m_type = MessageType::MOVE;
                    message->m_movement = MovementType::NO_MOVEMENT;
                }
            }
            return message;
        }
        );
        inputHandler->RegisterHandler(
            SDLK_SPACE,
            [this](InputComponent* inputComponent, std::unordered_map<SDL_Keycode, bool>& keycodes, Uint32 type)
        {
            CreateProjectile* message = nullptr;
            if ((type == SDL_KEYDOWN) && (!keycodes[SDLK_SPACE]))
            {
                if (!m_playerTemporaryDisabled)
                {
                    keycodes[SDLK_SPACE] = true;
                    Event* event = EventPool::GetEvent(CREATE_PROJECTILE);
                    if (event)
                    {
                        message = event->GetMessage<CreateProjectile>();
                    }
                    else
                    {
                        message = new CreateProjectile();
                    }
                    message->m_type = MessageType::CREATE_PROJECTILE;
                    message->m_projetileSprites = "data/sprite/projectile/projectile.json";
                    message->m_newEntity = m_entityManager->CreateEntity();
                }
            }
            else
            {
                keycodes[SDLK_SPACE] = false;
            }
            return message;
        }
        );
        inputHandler->RegisterHandler(
            SDLK_LCTRL,
            [this](InputComponent* inputComponent, std::unordered_map<SDL_Keycode, bool>& keycodes, Uint32 type)
        {
            Crouch* message = nullptr;
            if ((type == SDL_KEYDOWN) && (!keycodes[SDLK_LCTRL]))
            {
                if (!m_playerTemporaryDisabled)
                {
                    keycodes[SDLK_LCTRL] = true;
                    Event* event = EventPool::GetEvent(CROUCH);
                    if (event)
                    {
                        message = event->GetMessage<Crouch>();
                    }
                    else
                    {
                        message = new Crouch();
                    }
                    message->m_type = MessageType::CROUCH;
                }
            }
            else if (type == SDL_KEYUP)
            {
                keycodes[SDLK_LCTRL] = false;
                Event* event = EventPool::GetEvent(CROUCH);
                if (event)
                {
                    message = event->GetMessage<Crouch>();
                }
                else
                {
                    message = new Crouch();
                }
                message->m_type = MessageType::CROUCH;
            }
            return message;
        }
        );
        keycodes.clear();
        keycodes = { SDLK_RIGHT, SDLK_LEFT, SDLK_SPACE, SDLK_DOWN, SDLK_LCTRL };
        inputSystem->CreateComponent(ComponentType::BasicInput, m_playerEntity, inputHandler, keycodes);
    }

    void GameSystem::Update()
    {
        if (m_levelOver)
        {
            m_systemsMap[SystemType::Camera]->Disable(-1);
            m_systemsMap[SystemType::Input]->Disable(m_gameEntity);
            m_systemsMap[SystemType::Collision]->Disable(-1);
            m_systemsMap[SystemType::Movement]->Disable(-1);
            m_systemsMap[SystemType::Render]->Disable(m_playerEntity);
            m_levelOver = false;
        }
        if (m_levelFinished)
        {
            m_systemsMap[SystemType::Camera]->Disable(-1);
            m_systemsMap[SystemType::Input]->Disable(m_gameEntity);
            m_systemsMap[SystemType::Collision]->Disable(-1);
            m_systemsMap[SystemType::Movement]->Disable(-1);
            m_levelFinished = false;
        }
        for (const auto& system : m_systems)
        {
            system->Update();
        }
    }

    void GameSystem::DrawHUD()
    {
        SDL_Rect rect = { 0, 550, 800, 50 };
        SDL_RenderCopy(m_renderer, m_hudTexture, NULL, &rect);

        rect = { 0, 0, 800, 40 };
        SDL_RenderCopy(m_renderer, m_hudTexture, NULL, &rect);

        int offset = 40;
        for (int i = 0; i < m_playerLives; i++)
        {
            rect = { 20 + (offset * i), 560, 30, 30 };
            SDL_RenderCopy(m_renderer, m_hudLives, NULL, &rect);
        }
    }

    void GameSystem::Receive(Event* event)
    {
        if (event->m_type == COLLISION_HAPPENED)
        {
            CollisionHappened* collisionHappend = event->GetMessage<CollisionHappened>();
            if (((collisionHappend->m_firstColComponentType == CharacterCollision) &&
                (collisionHappend->m_secondColComponentType == ProjectileCollision)) ||
                ((collisionHappend->m_secondColComponentType == CharacterCollision) &&
                (collisionHappend->m_firstColComponentType == ProjectileCollision)))
            {
                SoundManager::PlaySFXMusic(m_hit);
                if ((collisionHappend->m_firstColEntity == m_playerEntity) || (collisionHappend->m_secondColEntity == m_playerEntity))
                {
                    --m_playerLives;
                    if (m_playerLives == 0)
                    {
                        PlayerEliminated* playerEliminated;
                        Event* event = EventPool::GetEvent(PLAYER_ELIMINATED);
                        if (event)
                        {
                            playerEliminated = event->GetMessage<PlayerEliminated>();
                        }
                        else
                        {
                            playerEliminated = new PlayerEliminated();
                        }
                        playerEliminated->m_type = PLAYER_ELIMINATED;
                        playerEliminated->m_entity = m_playerEntity;
                        Notify(playerEliminated);
                        EventPool::AddEvent(playerEliminated);
                        m_levelOver = true;
                        SoundManager::PauseBackGroundMusic();
                        SoundManager::PlaySFXMusic(m_failed);
                    }
                }
            }
            else if (((collisionHappend->m_firstColEntity == m_playerEntity) && (collisionHappend->m_firstColComponentType == CharacterCollision) && (collisionHappend->m_secondColComponentType == StaticCollision))
                || ((collisionHappend->m_secondColEntity == m_playerEntity) && (collisionHappend->m_secondColComponentType == CharacterCollision) && (collisionHappend->m_firstColComponentType == StaticCollision)))
            {
                m_playerPosition = m_playerLastPosition;
            }
        }
        else if ((event->m_type == POSITION_CHANGE) && (event->m_entity == m_playerEntity))
        {
            ChangePoisition* changePosition = event->GetMessage<ChangePoisition>();
            m_playerLastPosition = m_playerPosition;
            m_playerPosition = changePosition->m_absolutePosition;
        }
        else if (event->m_type == PLAYER_ELIMINATED)
        {
            if (event->m_entity == m_playerEntity)
            {
                CreateLevel(dynamic_cast<RenderSystem*>(m_systemsMap[SystemType::Render]), dynamic_cast<CollisionSystem*>(m_systemsMap[SystemType::Collision]),
                    dynamic_cast<MovementSystem*>(m_systemsMap[SystemType::Movement]), dynamic_cast<InputSystem*>(m_systemsMap[SystemType::Input]),
                    dynamic_cast<CameraSystem*>(m_systemsMap[SystemType::Camera]));
                m_systemsMap[SystemType::Camera]->Enable();
                m_systemsMap[SystemType::Input]->Enable();
                m_systemsMap[SystemType::Collision]->Enable();
                m_systemsMap[SystemType::Movement]->Enable();
                m_systemsMap[SystemType::Render]->Enable();
                m_playerLives = 3;
                m_playerHorizontalPosition = 0;
                SoundManager::StopSFXMusic();
                SoundManager::ResumeBackGroundMusic();
            }
            else
            {
                if (m_bonusScore)
                {
                    m_bonusScore = false;
                    m_playerScore += 500;
                }
                else
                {
                    m_playerScore += 100;
                }
            }
        }
        else if (event->m_type == PLAYER_FINISHED)
        {
            if (event->m_entity == m_playerEntity)
            {
                DriveCar* driveCar;
                Event* event = EventPool::GetEvent(DRIVE_CAR);
                if (event)
                {
                    driveCar = event->GetMessage<DriveCar>();
                }
                else
                {
                    driveCar = new DriveCar();
                }
                driveCar->m_type = DRIVE_CAR;
                driveCar->m_entity = m_carEntity;
                Notify(driveCar);
                EventPool::AddEvent(driveCar);
                m_drawScore = true;
                SoundManager::PauseBackGroundMusic();
                SoundManager::PlaySFXMusic(m_car);
            }
        }
        else if (event->m_type == DRIVE_CAR)
        {
            m_systemsMap[SystemType::Render]->Disable(-1);
            CreateLevel(dynamic_cast<RenderSystem*>(m_systemsMap[SystemType::Render]), dynamic_cast<CollisionSystem*>(m_systemsMap[SystemType::Collision]),
                dynamic_cast<MovementSystem*>(m_systemsMap[SystemType::Movement]), dynamic_cast<InputSystem*>(m_systemsMap[SystemType::Input]),
                dynamic_cast<CameraSystem*>(m_systemsMap[SystemType::Camera]));
            m_systemsMap[SystemType::Camera]->Enable();
            m_systemsMap[SystemType::Input]->Enable();
            m_systemsMap[SystemType::Collision]->Enable();
            m_systemsMap[SystemType::Movement]->Enable();
            m_systemsMap[SystemType::Render]->Enable();
            m_playerLives = 3;
            m_playerHorizontalPosition = 0;
            SoundManager::ResumeBackGroundMusic();
        }
        else if (event->m_type == MOVE_UP)
        {
            MoveUp* moveUp = event->GetMessage<MoveUp>();
            if (((m_buildingLevel * 95.0) + 40) <= (m_playerHorizontalPosition + moveUp->m_moveLength))
            {
                ++m_buildingLevel;
                SpawnEnemy* message;
                Event* event = EventPool::GetEvent(SPAWN_ENEMY);
                if (event)
                {
                    message = event->GetMessage<SpawnEnemy>();
                }
                else
                {
                    message = new SpawnEnemy();
                }
                message->m_type = SPAWN_ENEMY;
                message->m_position = glm::dvec2(0.0, ((m_buildingLevel * 95.0) + 85.0) - (m_playerHorizontalPosition));
                message->m_entity = m_playerEntity;
                Notify(message);
                EventPool::AddEvent(message);
            }
            m_playerHorizontalPosition += moveUp->m_moveLength;

            if (m_playerHorizontalPosition >= (30.0 * 95.0))
            {
                PlayerFinished* playerFinished;
                Event* event = EventPool::GetEvent(PLAYER_FINISHED);
                if (event)
                {
                    playerFinished = event->GetMessage<PlayerFinished>();
                }
                else
                {
                    playerFinished = new PlayerFinished();
                }
                playerFinished->m_type = PLAYER_FINISHED;
                playerFinished->m_entity = m_playerEntity;
                Notify(playerFinished);
                EventPool::AddEvent(playerFinished);
                m_levelFinished = true;
            }
        }
        else if (event->m_type == EXIT_GAME)
        {
            m_gameOver = true;
        }
        else if (event->m_type == MessageType::PLAYER_HIDDEN)
        {
            if (m_playerTemporaryDisabled)
            {
                m_playerTemporaryDisabled = false;
                m_bonusScore = true;
            }
            else
            {
                m_playerTemporaryDisabled = true;
            }
        }
    }

    void GameSystem::RestartLevel()
    {}

    void GameSystem::Draw()
    {
        DrawHUD();
        // draw score
        char msg[1024];
        sprintf(msg, "%07d", m_playerScore);
        DrawText(50, 10, msg);

        //draw end score
        if (m_drawScore)
        {
            DrawText(290, 280, "GAME OVER");
            sprintf(msg, "SCORE-%07d", m_playerScore);
            DrawText(230, 350, msg);
        }

        SwapBuffers();
        ClearWindow();
    }

    void GameSystem::SwapBuffers()
    {
        SDL_RenderPresent(m_renderer);
    }

    void GameSystem::ClearWindow()
    {
        SDL_RenderClear(m_renderer);
    }

    void GameSystem::DrawText(int x, int y, const char * msg)
    {
        SDL_Color white = { 255, 255, 255 };  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color

        SDL_Surface* surf = TTF_RenderText_Solid(m_font, msg, white); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

        SDL_Texture* msg_texture = SDL_CreateTextureFromSurface(m_renderer, surf); //now you can convert it into a texture

        int w = 0;
        int h = 0;
        SDL_QueryTexture(msg_texture, NULL, NULL, &w, &h);
        SDL_Rect dst_rect = { x, y, w, h };

        SDL_RenderCopy(m_renderer, msg_texture, NULL, &dst_rect);

        SDL_DestroyTexture(msg_texture);
        SDL_FreeSurface(surf);
    }

    bool GameSystem::GameOver()
    {
        return m_gameOver;
    }

    float GameSystem::GetElapsedTime()
    {
        return SDL_GetTicks();
    }

    void GameSystem::DisableComponents(int excludeEntity)
    {
    }

    void GameSystem::Destroy()
    {
        SDL_Log("Shutting down the game\n");
        for (auto system : m_systems)
        {
            system->Destroy();
            delete system;
        }
        EventPool::DestroyEvents();
        Mix_CloseAudio();
        SDL_DestroyTexture(m_hudLives);
        SDL_DestroyTexture(m_hudTexture);
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);

        TTF_CloseFont(m_font);
        TTF_Quit();

        SDL_Quit();

        delete m_entityManager;
    }
}