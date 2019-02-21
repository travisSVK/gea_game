#include "GameSystem.hpp"
#include "EntityManager.hpp"
#include "systems/MovementSystem.hpp"
#include "systems/RenderSystem.hpp"
#include "common/Event.hpp"
#include "components/SpritesheetRenderComponent.hpp"
#include "components/SpriteRenderComponent.hpp"
#include "components/PlayerMovementComponent.hpp"
#include "components/ProjectileMovementComponent.hpp"
#include "handlers/EventHandler.hpp"

GameSystem::GameSystem() : System()
{
    m_systems.reserve(5);
}

bool GameSystem::Init(int width, int height)
{
    SDL_Log("Initializing the engine...\n");

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL failed the initialization: %s\n", SDL_GetError());
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
    font = TTF_OpenFont("data/EA.ttf", 12); //this opens a font style and sets a size
    if (font == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "font cannot be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // initialize the keys
    key.fire = false;	key.left = false;	key.right = false, key.esc = false;

    //Initialize renderer color
    SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    //Clear screen
    SDL_RenderClear(m_renderer);

    SDL_Log("Engine up and running...\n");

    m_gameOver = false;
    m_playerMoving = false;
    return true;
}

void GameSystem::Create()
{
    // create systems

    EventHandler<MovementSystem, MovementComponent>* movementEventHandler = new EventHandler<MovementSystem, MovementComponent>();
    movementEventHandler->RegisterHandler(
        MessageType::MOVE,
        [](MovementSystem* renderSystem, const std::unordered_map<EntityManager::Entity, MovementComponent*> componentsMap, Event* event)
    {
        componentsMap.at(event->m_entity)->Receive(event->m_message);
    }
    );


    MovementSystem* movementSystem = new MovementSystem(movementEventHandler);
    m_systems.push_back(movementSystem);
    //TODO create input handler system and subscribe to that one instead
    Subscribe([movementSystem](Event* event) {movementSystem->Receive(event); });
    movementSystem->Register(MovementComponentType::PlayerMovement);
    movementSystem->Register(MovementComponentType::ProjectileMovement);

    EventHandler<RenderSystem, RenderComponent>* renderEventHandler = new EventHandler<RenderSystem, RenderComponent>();
    renderEventHandler->RegisterHandler(
        MessageType::POSITION_CHANGE,
        [](RenderSystem* renderSystem, const std::unordered_map<EntityManager::Entity, RenderComponent*> componentsMap, Event* event)
    {
        ChangePoisition* changePosition = event->m_message->GetMessage<ChangePoisition>();
        componentsMap.at(event->m_entity)->SetPosition(changePosition->m_positionX, changePosition->m_positionY);
    }
    );

    renderEventHandler->RegisterHandler(
        MessageType::MOVE,
        [](RenderSystem* renderSystem, const std::unordered_map<EntityManager::Entity, RenderComponent*> componentsMap, Event* event)
    {
        componentsMap.at(event->m_entity)->Receive(event->m_message);
    }
    );

    RenderSystem* renderSystem = new RenderSystem(renderEventHandler);
    m_systems.push_back(renderSystem);
    //TODO create input handler system and subscribe to that one instead
    Subscribe([renderSystem](Event* event) {renderSystem->Receive(event); });
    movementSystem->Subscribe([renderSystem](Event* event) {renderSystem->Receive(event); });
    renderSystem->Register(RenderComponentType::Spritesheet);
    renderSystem->Register(RenderComponentType::Sprite);

    // create entities
    m_entityManager = new EntityManager();
    m_playerEntity = m_entityManager->CreateEntity();
    movementSystem->CreateComponent<PlayerMovementComponent>(MovementComponentType::PlayerMovement, m_playerEntity, 50.0, 50.0, DirectionType::DIR_RIGHT);
    renderSystem->CreateComponent<SpritesheetRenderComponent>(RenderComponentType::Spritesheet, m_playerEntity, m_renderer, "data/sprite/player", 50.0, 50.0);
}

void GameSystem::Update(float delta)
{
    // TODO move processing input into input proessing system
    ProcessInput();
    if (key.esc)
    {
        m_gameOver = true;
        return;
    }
    if (key.left)
    {
        m_playerMoving = true;
        Move* message = new Move();
        message->m_movement = MovementType::LEFT;
        message->m_type = MessageType::MOVE;
        Event* event = new Event();
        event->m_entity = m_playerEntity;
        event->m_type = MessageType::MOVE;
        event->m_message = message;
        Notify(event);
        delete event;
    }
    else if (key.right)
    {
        m_playerMoving = true;
        Move* message = new Move();
        message->m_movement = MovementType::RIGHT;
        message->m_type = MessageType::MOVE;
        Event* event = new Event();
        event->m_entity = m_playerEntity;
        event->m_type = MessageType::MOVE;
        event->m_message = message;
        Notify(event);
        delete event;
    }
    else
    {
        if (m_playerMoving)
        {
            Move* message = new Move();
            message->m_type = MessageType::MOVE;
            message->m_movement = MovementType::NO_MOVEMENT;
            Event* event = new Event();
            event->m_entity = m_playerEntity;
            event->m_type = MessageType::MOVE;
            event->m_message = message;
            Notify(event);
            delete event;
            m_playerMoving = false;
        }
    }
    if (key.fire)
    {
        //EntityManager::Entity entity = m_entityManager->CreateEntity();
        //// TODO send event SpawnedFromEntity
        //CreateMovementComponent* movementEvent = new CreateMovementComponent();
        //movementEvent->m_entity = entity;
        //movementEvent->m_sourceEntity = m_playerEntity;
        //movementEvent->m_componentType = (CreateMovementComponent::MovementComponentType)MovementSystem::ProjectileMovement;
        //Notify(movementEvent);
        //delete movementEvent;

        //m_systemsMap[SystemType::Render]->CreateComponent<SpriteRenderComponent>(ComponentType::Sprite, entity, m_renderer, "data/sprite/projectile.png");
        //// TODO send event SpawnedFromEntity
        //CreateRenderComponent<>* renderEvent = new CreateRenderComponent<>(entity, m_renderer, "data/sprite/projectile.png");
        //renderEvent->m_entity = entity;
        //renderEvent->m_sourceEntity = m_playerEntity;
        //renderEvent->m_renderer = m_renderer;
        ////renderEvent->m_componentType = (CreateRenderComponent::RenderComponentType)RenderSystem::Sprite;
        //renderEvent->m_path = "data/sprite/projectile.png";
        //Notify(renderEvent);
        //delete renderEvent;
    }

    for (const auto& system : m_systems)
    {
        system->Update(delta);
    }
}

void GameSystem::Receive(Event* event)
{
    // TODO receive game over events etc
}

void GameSystem::Draw()
{
    // TODO draw things here
    char msg[1024];
    /*sprintf(msg, "%07d", Score());
    engine->drawText(300, 32, msg);
    sprintf(msg, "bonus: %.1fX", game_speed);
    engine->drawText(510, 32, msg);

    for (int i = 0; i < player->lives; i++)
        life_sprite->draw(i * 36 + 20, 16);*/

    if (m_gameOver)
    {
        sprintf(msg, "*** G A M E  O V E R ***");
        DrawText(250, 8, msg);
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

void GameSystem::ProcessInput()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
            case SDLK_q:
                key.esc = true;
                break;
            case SDLK_SPACE:
                key.fire = true;
                break;
            case SDLK_LEFT:
                key.left = true;
                break;
            case SDLK_RIGHT:
                key.right = true;
                break;
            }
        }

        if (event.type == SDL_KEYUP)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_SPACE:
                key.fire = false;
                break;
            case SDLK_LEFT:
                key.left = false;
                break;
            case SDLK_RIGHT:
                key.right = false;
                break;
            }
        }
    }
}

void GameSystem::DrawText(int x, int y, const char * msg)
{
    SDL_Color black = { 0, 0, 0 };  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color

    SDL_Surface* surf = TTF_RenderText_Solid(font, msg, black); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

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
    return SDL_GetTicks() / 1000.f;
}

void GameSystem::Destroy()
{
    SDL_Log("Shutting down the engine\n");

    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);

    TTF_CloseFont(font);

    TTF_Quit();
    SDL_Quit();
}