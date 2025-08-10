#pragma once
#include "State.hpp"
#include "GameObject.hpp"
class Player;
class PlayerState : public State
{
protected:
    Player *mario;

    // 16 pixels = 1 meter

    const float walkSpeed = 5.625f;
    const float runSpeed = 9.84375f;

    const float fallSpeedCap = 15.0f;

    const float walkAccel = 14.0625f;
    const float runAccel = 21.09375f;
    const float airAccel = 7.03125f;

    const float friction = 14.0625f;
    const float airFriction = 3.125f;
    const float skidDecel = 56.25f;

    const float speedTransition = 0.3f;

public:
    PlayerState(int Type, Player *_mario, int _delay);

    // direction : facing left or right
    // speedCap : the maximum speed
    // accel : the horizontal acceleration
    virtual void HorizontalAccelerate(float speedCap, float accel);
};

class IdleState : public PlayerState
{
public:
    IdleState(Player *_mario, int _delay = 5);
    void handleInput() override;
};

class WalkState : public PlayerState
{
public:
    WalkState(Player *_mario, int _delay = 5);
    void handleInput() override;
};
class RunState : public PlayerState
{
public:
    RunState(Player *_mario, int _delay = 3);
    void handleInput() override;
};
class JumpState : public PlayerState
{
public:
    JumpState(Player *_mario, int _delay = 5);
    void handleInput() override;
};
class FallState : public PlayerState
{
public:
    FallState(Player *_mario, int _delay = 5);
    void handleInput() override;
};
class SkidState : public PlayerState
{
public:
    SkidState(Player *_mario, int _delay = 5);
    void handleInput() override;
};
class GrowState : public PlayerState
{
private:
    float posBeforeY = 0;

public:
    GrowState(Player *_mario, int _delay = 3);
    void handleInput() override;
};
class UnGrowState : public PlayerState
{
private:
    float posBeforeY = 0;

public:
    UnGrowState(Player *_mario, int _delay = 3);
    void handleInput() override;
};
class CrouchState : public PlayerState
{
public:
    CrouchState(Player *_mario, int _delay = 5);
    void handleInput() override;
};

class DeadState : public PlayerState
{
public:
    DeadState(Player *_mario, int _delay = 5);
    void handleInput() override;
};
