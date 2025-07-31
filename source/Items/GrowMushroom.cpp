#include "GrowMushroom.hpp"

GrowMushroom::GrowMushroom() {
	setFrame(itemStateType::MOVE, 0, 0);
	setTexture("Items");
}

void GrowMushroom::affect(Mario* mario) {
	mario->changeState(new GrowState(mario));
}