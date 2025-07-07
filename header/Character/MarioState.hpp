#pragma once
#include "State.hpp"
class MarioState : public State
{
protected:
    // Speeds (px/sec)
    const float walkSpeed = 90.0f;
    const float runSpeed = 157.5f;
    const float jumpVel = -390.0f;

    // Accelerations (px/sec�)
    const float walkAccel = 225.0f;
    const float airAccel = 112.5f;
    const float runAccel = 337.5f;
    const float jumpGravity = 1056.25f; // While holding jump
    const float fallGravity = 1462.5f;  // Letting go / falling
    const float friction = 225.0f;
    const float airFriction = 50.0f; // much lower than ground friction
    const float skidDecel = 900.0f;

public:
    MarioState(stateType Type, Character *_character, int _delay) : State(Type, _character, _delay) {}
};
class IdleState : public MarioState
{
public:
    IdleState(Character *_character, int _delay = 5);
    void handleInput() override;
};

class WalkState : public MarioState
{
public:
    WalkState(Character *_character, int _delay = 5);
    void handleInput() override;
};
class RunState : public MarioState
{
public:
    RunState(Character *_character, int _delay = 3);
    void handleInput() override;
};
class JumpState : public MarioState
{
public:
    JumpState(Character *_character, int _delay = 5);
    void handleInput() override;
};
class FallState : public MarioState
{
public:
    FallState(Character *_character, int _delay = 5);
    void handleInput() override;
};
class SkidState : public MarioState
{
public:
    SkidState(Character *_character, int _delay = 5);
    void handleInput() override;
};
class GrowState : public MarioState
{
public:
    GrowState(Character *_character, int _delay = 5);
    void handleInput() override;
};
class UnGrowState : public MarioState
{
public:
    UnGrowState(Character *_character, int _delay = 3);
    void handleInput() override;
};
class CrouchState : public MarioState
{
public:
    CrouchState(Character *_character, int _delay = 5);
    void handleInput() override;
};
