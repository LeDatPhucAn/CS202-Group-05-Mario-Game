#include "Character.hpp"
void Character::changeState(State* newState) {
	if (currentState)delete currentState;
	currentState = newState;
}
void Character::update() {
	if (currentState) {
		currentState->updateState();
	}
    pos = movement.pos;
}
void Character::display() {
	if (currentState) {
		currentState->displayState();
	}
}
void Character::changeForm(MarioForm form) {
    switch (form) {
    case SMALL:
        sprite.StartEndFrames[IDLE] = { 0, 0 };
        sprite.StartEndFrames[JUMP] = { 5, 5 };
        sprite.StartEndFrames[FALL] = { 5, 5 };
        sprite.StartEndFrames[SKID] = { 4, 4 };
        sprite.StartEndFrames[WALK] = { 1, 3 };
        sprite.StartEndFrames[RUN] = { 1, 3 };
        sprite.StartEndFrames[CROUCH] = { 0, 0 };
        sprite.StartEndFrames[GROW] = { 44, 50 };
        sprite.StartEndFrames[UNGROW] = { 0, 0 };
        break;
    case BIG:
        sprite.StartEndFrames[IDLE] = { 13, 13 };
        sprite.StartEndFrames[WALK] = { 16, 14 };
        sprite.StartEndFrames[JUMP] = { 18, 18 };
        sprite.StartEndFrames[FALL] = { 18, 18 };
        sprite.StartEndFrames[SKID] = { 17, 17 };
        sprite.StartEndFrames[RUN] = { 16, 14 };
        sprite.StartEndFrames[CROUCH] = { 19, 19 };
        sprite.StartEndFrames[GROW] = { 28, 28 };
        sprite.StartEndFrames[UNGROW] = { 50, 44 };
        break;
    case FIRE:
        sprite.StartEndFrames[IDLE] = { 28, 28 };
        sprite.StartEndFrames[WALK] = { 31, 29 };
        sprite.StartEndFrames[JUMP] = { 34, 34 };
        sprite.StartEndFrames[FALL] = { 34, 34 };
        sprite.StartEndFrames[SKID] = { 33, 33 };
        sprite.StartEndFrames[RUN] = { 31, 29 };
        sprite.StartEndFrames[CROUCH] = { 35, 35 };
        sprite.StartEndFrames[GROW] = { 28, 28 };
        sprite.StartEndFrames[UNGROW] = { 13, 13 };
        break;
    default:
        break;
    }
}