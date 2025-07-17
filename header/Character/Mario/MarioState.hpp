#pragma once
#include "State.hpp"
class Mario;
class MarioState : public State
{
protected:
    Mario *mario;

    // 16 pixels = 1 meter

    const float walkSpeed = 5.625f;
    const float runSpeed = 9.84375f;
    const float jumpVel = -14.85f; // For 4.5m jump
    const float fallSpeedCap = 15.0f;

    const float walkAccel = 14.0625f;
    const float runAccel = 21.09375f;
    const float airAccel = 7.03125f;

    const float friction = 14.0625f;
    const float airFriction = 3.125f;
    const float skidDecel = 56.25f;

    const float fallGravity = 14.7f;
    const float jumpGravity = 9.8f;

    const float maxJumpHeight = 4.5f;

public:
    MarioState(stateType Type, Mario *_mario, int _delay);

    // direction : facing left or right
    // speedCap : the maximum speed
    // accel : the horizontal acceleration
    virtual void HorizontalAccelerate(float speedCap, float accel);
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

class DeadState : public MarioState
{
public:
    DeadState(Mario *_mario, int _delay = 5);
    void handleInput() override;
};