#pragma once
#include "State.hpp"
class Mario;
class MarioState : public State
{
protected:
    Mario *mario;

    // PPM IS PIXEL PER METER

    // Movement Speeds (in meters/second)
    const float walkSpeed = 90.0f / PPM; // 2.8125f
    const float runSpeed = 157.5f / PPM; // 4.921875f
    const float jumpVel = -390.0f / PPM; // -12.1875f
    const float fallSpeedCap = 240.0f / PPM;
    // Accelerations (in meters/second^2)
    const float walkAccel = 225.0f / PPM; // 7.03125f
    const float airAccel = 112.5f / PPM;  // 3.515625f
    const float runAccel = 337.5f / PPM;  // 10.546875f

    const float friction = 225.0f / PPM;   // 7.03125f
    const float airFriction = 50.0f / PPM; // 1.5625f
    const float skidDecel = 900.0f / PPM;  // 28.125f

    // Max jump height (in meters)
    const float maxJumpHeight = 72.0f / PPM; // 2.25f

public:
    MarioState(stateType Type, Mario *_mario, int _delay);

    // direction : facing left or right
    // speedCap : the maximum speed
    // accel : the horizontal acceleration
    virtual void HorizontalAccelerate(Direction direction, float speedCap, float accel);
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