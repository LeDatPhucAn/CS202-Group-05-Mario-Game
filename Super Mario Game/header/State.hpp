#pragma once
#include "raylib.h"
#include "raymath.h"
enum stateType {
    IDLE,
    WALK
};
class Character;
class State {
protected:
    int numFrames = 0;
    int frameIndex = 0;
    int delay = 0;
    int delayCounter = 0;
    Rectangle frameRec;
    stateType type;
    Character* character = nullptr;
public:
    State();
    State(Character* _character, int _delay);
    virtual void animate();
    virtual void updateState();
    virtual void displayState() = 0;
    virtual void handleInput() = 0;

};

class IdleState : public State {
public:
    IdleState();
    IdleState(Character* _character, int _delay=0);
    void handleInput() override;
    //void updateState() override;
    void displayState() override;
};

class WalkState : public State {
public:
    WalkState();
    WalkState(Character* _character, int _delay=5);
    void handleInput() override;
    //void updateState() override;
    void displayState() override;
};
