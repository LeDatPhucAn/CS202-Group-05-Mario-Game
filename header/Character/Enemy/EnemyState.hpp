
#pragma once
#include "State.hpp"

class Character;

class EnemyState : public State {
protected:
    Character* character;
public:
    EnemyState(stateType type, Character* character, int delay = 5);
    virtual void handleInput() = 0;
};

class EnemyIdleState : public EnemyState {
public:
    EnemyIdleState(Character* character, int delay = 10);
    void handleInput() override;
};
class EnemyWalkState : public EnemyState {
public:
    EnemyWalkState(Character* character, int delay = 15);
    void handleInput() override;
};

class EnemyRunState : public EnemyState {
private:
    
public:
    EnemyRunState(Character* character, int delay = 5);
    void handleInput() override;
};

class EnemyDeadState : public EnemyState {
private:
    float delayCounter = 1.0f;
public:
    EnemyDeadState(Character* character, int delay = 5);
    void handleInput() override;
    void updateState() override;
};
class EnemyFlyState : public EnemyState
{
private:
    float flyTime = 0.0f;
    float flyAmplitude = 50.0f; // How high/low it flies
    float flyFrequency = 2.0f;  // How fast it oscillates
    float directionTimer = 0.0f; 
    
public:
    EnemyFlyState(Character *_character, int _delay = 500);
    void handleInput() override;
    void updateState() override;
};



