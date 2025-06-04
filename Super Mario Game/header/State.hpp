#pragma once
#include "raylib.h"
#include "raymath.h"
enum stateType {
    IDLE,
    WALK,
    JUMP,
    FALL
};
class Character;
class State {
protected:
    const int gravity = 1;
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
    virtual void animate();
    virtual void updateState();
    virtual void displayState();
    virtual void handleInput() = 0;

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
