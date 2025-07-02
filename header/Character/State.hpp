#pragma once
#include "raylib.h"
#include "raymath.h"
enum MarioForm {
    SMALL = 0,
    BIG = 1,
    FIRE,
	FORM_COUNT
}; 
enum stateType {
    IDLE,
    WALK,
    RUN,
    JUMP,
    FALL,
    SKID,
    CROUCH,
    GROW,
    UNGROW
};
enum Direction {
    LEFT = -1,
    BRUH,
    RIGHT = 1,
};
class Character;
class State {
protected:

    // pos
    const float maxHeight = 72.0f;
    const float GroundPosY = 208.0f;
    // Speeds (px/sec)
    const float walkSpeed = 90.0f;
    const float runSpeed = 157.5f;
    const float fallSpeedCap = 240.0f;
    const float jumpVel = -390.0f;

    // Accelerations (px/sec�)
    const float walkAccel = 225.0f;
    const float airAccel = 112.5f;
    const float runAccel = 337.5f;
    const float jumpGravity = 1056.25f;   // While holding jump
    const float fallGravity = 1462.5f;  // Letting go / falling
    const float friction = 225.0f;
    const float skidDecel = 900.0f;

    // basic parameters
    int numFrames = 0;
    int frameIndex = 0;
    int delay = 0;
    int delayCounter = 0;
    stateType type;
    Rectangle frameRec;
    Character* character = nullptr;
public:
    State();
    State(stateType Type, Character* _character, int _delay);

    // salient 
    virtual void updateState();
    virtual void displayState();
    virtual void handleInput() = 0;

    // constantly changing frames to run sprite
    virtual void animate();

    // manage gravity and movements
    virtual void applyPhysics(float deltaTime);
    

};

class IdleState : public State {
public:
    IdleState();
    IdleState(Character* _character, int _delay=5);
    void handleInput() override;
};

class WalkState : public State {
public:
    WalkState();
    WalkState(Character* _character, int _delay=5);
    void handleInput() override;
};
class RunState : public State {
public:
    RunState();
    RunState(Character* _character, int _delay=3);
    void handleInput() override;
};
class JumpState : public State {
public:
    JumpState();
    JumpState(Character* _character, int _delay=5);
    void handleInput() override;
};
class FallState : public State {
public:
    FallState();
    FallState(Character* _character, int _delay=5);
    void handleInput() override;
};
class SkidState : public State {
public:
    SkidState();
    SkidState(Character* _character, int _delay=5);
    void handleInput() override;
};
class GrowState : public State {
public:
    GrowState();
    GrowState(Character* _character, int _delay=5);
    void handleInput() override;
};
class UnGrowState : public State {
public:
    UnGrowState();
    UnGrowState(Character* _character, int _delay=3);
    void handleInput() override;
};
class CrouchState : public State {
    public:
    CrouchState();
    CrouchState(Character* _character, int _delay=5);
    void handleInput() override;
};
