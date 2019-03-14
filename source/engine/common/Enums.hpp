#pragma once

enum ComponentType
{
    SpriteRender = 0,
    SpritesheetRender = 1,
    StaticRender = 2,
    DoorRender = 3,
    PlayerMovement = 5,
    AIMovement = 6,
    ProjectileMovement = 7,
    ElevatorMovement = 8,
    BasicInput = 11,
    ProjectileCollision = 15,
    StaticCollision = 16,
    CharacterCollision = 17,
    HorizontalCamera = 20
};

enum SystemType
{
    Render = 0,
    Movement = 1,
    Input = 2,
    Collision = 3,
    Camera = 4
};

enum MovementType
{
    LEFT = 0,
    RIGHT = 1,
    NO_MOVEMENT_RIGHT = 2,
    NO_MOVEMENT_LEFT = 3,
    NO_MOVEMENT = 4,
    DESTROYED_LEFT = 5,
    DESTROYED_RIGHT = 6,
    UP = 7,
    DOWN = 8,
    CAR_DRIVE = 9
};

enum DoorState
{
    CLOSED = 0,
    OPENING = 1,
    CLOSING = 2
};

enum DirectionType
{
    DIR_LEFT = 0,
    DIR_RIGHT = 1,
    DIR_UP = 2,
    DIR_DOWN = 3
};

enum MessageType
{
    NO_MESSAGE,
    MOVE,
    POSITION_CHANGE,
    CREATE_PROJECTILE,
    COLLISION_HAPPENED,
    MOVE_UP,
    EXCLUDE_ENTITY,
    INCLUDE_ENTITY,
    MOVE_ELEVATOR,
    PLAYER_ELIMINATED,
    PLAYER_FINISHED,
    DRIVE_CAR,
    OPEN_DOOR,
    PLAYER_HIDDEN,
    EXIT_GAME
};