#include "GrowMushroom.hpp"
#include "ItemState.hpp"

GrowMushroom::GrowMushroom() : Items() {
	setFrame(itemStateType::MOVE, 0, 0);
	setTexture("Items");
	changeState(new ItemMoveState(this));
}

void GrowMushroom::affect(Mario* mario) {
	mario->changeState(new GrowState(mario));
}

void GrowMushroom::update() {
	if (currentState) {
		currentState->updateState();
	}
}