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
    int frameIndex = 0;
    int numFrames = 0;
    int delay = 0;
    int delayCounter = 0;
    Character* character = nullptr;
    Rectangle frameRec = { 0 };
public:
    State();
    State(int _numFrames, Character* _character, int _delay);
    virtual void animate();
    virtual void updateState() = 0;
    virtual void displayState() = 0;
    virtual void handleInput() = 0;

    Rectangle getFrameRec() const { return frameRec; }
};

class IdleState : public State {
public:
    IdleState();
    IdleState(int _numFrames, Character* _character, int _delay=5);
    void handleInput() override;
    void updateState() override;
    void displayState() override;
};

class WalkState : public State {
public:
    WalkState();
    WalkState(int _numFrames, Character* _character, int _delay=5);
    void handleInput() override;
    void updateState() override;
    void displayState() override;
};
