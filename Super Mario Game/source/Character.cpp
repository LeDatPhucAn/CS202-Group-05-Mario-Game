#include "../header/Character.hpp"
void Character::changeState(State* newState) {
	if (currentState)delete currentState;
	currentState = newState;
}
void Character::update() {
	if (currentState) {
		currentState->updateState();
	}
}
void Character::draw() {
	currentState->displayState();
}
