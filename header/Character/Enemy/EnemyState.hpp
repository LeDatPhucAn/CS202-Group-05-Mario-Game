
// #pragma once
// #include "State.hpp"

// class Character;

// class EnemyState : public State {
// protected:
//     Character* character;
// public:
//     EnemyState(stateType type, Character* character, int delay = 5);
//     virtual void handleInput() = 0;
// };

// class EnemyIdleState : public EnemyState {
// public:
//     EnemyIdleState(Character* character, int delay = 5);
//     void handleInput() override;
// };
// class EnemyWalkState : public EnemyState {
// public:
//     EnemyWalkState(Character* character, int delay = 5);
//     void handleInput() override;
// };

// class EnemyRunState : public EnemyState {
// private:
    
// public:
//     EnemyRunState(Character* character, int delay = 5);
//     void handleInput() override;
// };

// class EnemyDeadState : public EnemyState {
// private:
//     float delayCounter = 1.0f;
// public:
//     EnemyDeadState(Character* character, int delay = 5);
//     void handleInput() override;
//     void updateState() override;
// };
