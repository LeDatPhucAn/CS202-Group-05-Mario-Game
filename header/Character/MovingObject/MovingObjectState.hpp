
#pragma once
#include "State.hpp"
#include "Structs.hpp"

class Character;

class MovingObjectState : public State
{
protected:
    Character *character;

public:
    MovingObjectState(int type, Character *character, int delay = 5);
    virtual void handleInput() = 0;
};

class MovingObjectIdleState : public MovingObjectState
{
public:
    MovingObjectIdleState(Character *character, int delay = 10);
    void handleInput() override;
};
class MovingObjectWalkState : public MovingObjectState
{
public:
    MovingObjectWalkState(Character *character, int delay = 15);
    void handleInput() override;
};

class MovingObjectRunState : public MovingObjectState
{
private:
public:
    MovingObjectRunState(Character *character, int delay = 5);
    void handleInput() override;
};
class FireBallMoveState : public MovingObjectState
{
private:
public:
    FireBallMoveState(Character *character, int delay = 5);
    void handleInput() override;
};
class LuigiFireBallMoveState : public MovingObjectState
{
private:
public:
    LuigiFireBallMoveState(Character *character, int delay = 5);
    void handleInput() override;
};
class StarMoveState : public MovingObjectState
{
private:
public:
    StarMoveState(Character *character, int delay = 5);
    void handleInput() override;
};

class MovingObjectDeadState : public MovingObjectState
{
private:
    float delayCounter = 1.0f;

public:
    MovingObjectDeadState(Character *character, int delay = 5);
    void handleInput() override;
    void updateState() override;
};
class MovingObjectStopState : public MovingObjectState
{
private:
    float delayCounter = 0.2f;

public:
    MovingObjectStopState(Character *character, int delay = 5);
    void handleInput() override;
    void updateState() override;
};
class MovingObjectFlyState : public MovingObjectState
{
public:
    MovingObjectFlyState(Character *character);
    ~MovingObjectFlyState();

    void handleInput() override;
    // This state is continuous and doesn't need an updateState to transition
};

class MovingObjectJumpState : public MovingObjectState
{
public:
    MovingObjectJumpState(Character *_character, int delay = 0);
    ~MovingObjectJumpState();

    void handleInput() override;
    void updateState() override;

private:
    float elapsed = 0.0f;
    const float period = 2.0f;          // seconds per full up‑and‑down cycle
    const float amplitude = 1.5f;       // max vertical displacement in meters
    const float horizontalSpeed = 1.5f; // m/s sideways
    const float omega = 2 * PI / 2.0f;  // angular frequency

    float baselineY = 0.0f; // world‑space y at state start

    void initSineJump();
};

class MovingObjectThrowState : public MovingObjectState
{
private:
    float throwDuration = 0.5f; // Duration of throw animation
    float throwTimer = 0.0f;
    bool hasThrown = false;

public:
    MovingObjectThrowState(Character *character, int delay = 5);
    void handleInput() override;
    void updateState() override;
    
    // Called by the character when it's time to actually throw
    virtual void executeThrow() = 0;
};