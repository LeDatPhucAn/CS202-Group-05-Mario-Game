
#pragma once
#include "State.hpp"
#include "Structs.hpp"

class Character;

class EnemyState : public State
{
protected:
    Character *character;

public:
    EnemyState(int type, Character *character, int delay = 5);
    virtual void handleInput() = 0;
};

class EnemyIdleState : public EnemyState
{
public:
    EnemyIdleState(Character *character, int delay = 10);
    void handleInput() override;
};
class EnemyWalkState : public EnemyState
{
public:
    EnemyWalkState(Character *character, int delay = 15);
    void handleInput() override;
};

class EnemyRunState : public EnemyState
{
private:
public:
    EnemyRunState(Character *character, int delay = 5);
    void handleInput() override;
};

class EnemyDeadState : public EnemyState
{
private:
    float delayCounter = 1.0f;

public:
    EnemyDeadState(Character *character, int delay = 5);
    void handleInput() override;
    void updateState() override;
};
class EnemyStopState : public EnemyState
{
private:
    float delayCounter = 0.2f;

public:
    EnemyStopState(Character *character, int delay = 5);
    void handleInput() override;
    void updateState() override;
};
class EnemyFlyState : public EnemyState
{
public:
    EnemyFlyState(Character *character);
    ~EnemyFlyState();

    void handleInput() override;
    // This state is continuous and doesn't need an updateState to transition
};

class EnemyJumpState : public EnemyState
{
public:
    EnemyJumpState(Character *_character, int delay = 0);
    ~EnemyJumpState();

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