#pragma once

enum RenderComponentType
{
    Sprite = 0,
    Spritesheet = 1
};

enum MovementComponentType
{
    PlayerMovement = 0,
    AIMovement = 1,
    ProjectileMovement = 2
};

enum SystemType
{
    Render = 0,
    Movement = 1
};

enum MovementType
{
    LEFT = 0,
    RIGHT = 1,
    NO_MOVEMENT_RIGHT = 2,
    NO_MOVEMENT_LEFT = 3,
    NO_MOVEMENT = 4
};

enum DirectionType
{
    DIR_LEFT = 0,
    DIR_RIGHT = 1
};

enum MessageType
{
    MOVE,
    POSITION_CHANGE,
    CREATE_MOVEMENT_COMPONENT,
    CREATE_RENDER_COMPONENT
};