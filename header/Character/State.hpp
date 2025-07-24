#pragma once
#include "raylib.h"
#include "raymath.h"
#include "Vec2Adapter.hpp"
enum MarioForm
{
    SMALL = 0,
    BIG = 1,
    FIRE,
    FORM_COUNT
};
enum stateType
{
    IDLE,
    WALK,
    RUN,
    JUMP,
    FALL,
    SKID,
    CROUCH,
    GROW,
    UNGROW,
    DEAD,
    FLY
};

enum Direction
{
    LEFT = -1,
    BRUH,
    RIGHT = 1,
};
class Character;
class State
{
private:
    friend class Character;
    friend class Mario;
protected:
    // basic parameters
    int numFrames = 0;
    int frameIndex = 0;
    int delay = 0;
    int delayCounter = 0;
    stateType type;
    Rectangle frameRec;
    Character *character = nullptr;

public:
    State();
    State(stateType Type, Character *_character, int _delay);

    // salient
    virtual void updateState();
    virtual void displayState();
    void DrawCharacterDebug(Character *character);

    // Đối với Enemy thì Input là Map collision
    // Đối với Mario thì Input từ bàn phím
    virtual void handleInput() = 0;

    // constantly changing frames to run sprite
    virtual void animate();
};
