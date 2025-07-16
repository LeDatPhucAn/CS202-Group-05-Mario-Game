#include "EnemyState.hpp"
#include "Enemy.hpp"
#include "Character.hpp"


EnemyState::EnemyState(stateType type, Character* _character, int _delay)
    : State(type, _character, _delay), character(_character) {}


EnemyIdleState::EnemyIdleState(Character* _character, int _delay)
    : EnemyState(IDLE, _character, _delay) {
    character->movement.velocity.x = 0.f;
    character->movement.acceleration.x = 0.f;
}

void EnemyIdleState::handleInput() {
}

EnemyWalkState::EnemyWalkState(Character* _character, int _delay)
    : EnemyState(WALK, _character, _delay) {
    character->movement.velocity.x = -20.f; 
    character->direction = RIGHT;
}

void EnemyWalkState::handleInput() {
}

EnemyRunState::EnemyRunState(Character* _character, int _delay)
    : EnemyState(RUN, _character, _delay) {
        if (character->direction == RIGHT) {
            character->movement.velocity.x = -100.f; 
        } else {
            character->movement.velocity.x = 100.f;
        }
}

void EnemyRunState::handleInput() {
    if (character->movement.velocity.x == 0) {
        character->direction = (character->direction == LEFT) ? RIGHT : LEFT;
        character->movement.velocity.x = (character->direction == LEFT) ? 100.f : -100.f;
    }
}


EnemyDeadState::EnemyDeadState(Character* _character, int _delay)
    : EnemyState(DEAD, _character, _delay) {
    character->movement.velocity = {0, 0};
    character->movement.acceleration = {0, 0};
}

void EnemyDeadState::updateState() {
    State::updateState(); 
    delayCounter -= GetFrameTime();
    if (delayCounter <= 0) {
        Enemy* enemy = dynamic_cast<Enemy*>(character);
        if (enemy) {
            enemy->beCleared = true;
        }
    }
}

void EnemyDeadState::handleInput() {
}

