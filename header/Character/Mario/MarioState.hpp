#pragma once
#include "State.hpp"
class Mario;
class MarioState : public State
{
protected:
    Mario *mario;
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
     MarioState(stateType Type, Mario *_mario, int _delay);
};
class IdleState : public MarioState
{
public:
    IdleState(Mario *_mario, int _delay = 5);
    void handleInput() override;
};

class WalkState : public MarioState
{
public:
    WalkState(Mario *_mario, int _delay = 5);
    void handleInput() override;
};
class RunState : public MarioState
{
public:
    RunState(Mario *_mario, int _delay = 3);
    void handleInput() override;
};
class JumpState : public MarioState
{
public:
    JumpState(Mario *_mario, int _delay = 5);
    void handleInput() override;
};
class FallState : public MarioState
{
public:
    FallState(Mario *_mario, int _delay = 5);
    void handleInput() override;
};
class SkidState : public MarioState
{
public:
    SkidState(Mario *_mario, int _delay = 5);
    void handleInput() override;
};
class GrowState : public MarioState
{
public:
    GrowState(Mario *_mario, int _delay = 5);
    void handleInput() override;
};
class UnGrowState : public MarioState
{
public:
    UnGrowState(Mario *_mario, int _delay = 3);
    void handleInput() override;
};
class CrouchState : public MarioState
{
public:
    CrouchState(Mario *_mario, int _delay = 5);
    void handleInput() override;
};

class DeadState : public MarioState {
public:
    DeadState(Mario* _mario, int _delay=5);
    void handleInput() override;
};