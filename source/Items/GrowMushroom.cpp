#include "GrowMushroom.hpp"

GrowMushroom::GrowMushroom() {
	setTexture("Items");
	setFrame(itemStateType::IDLE, 0, 0);
}

void GrowMushroom::affect(Mario* mario) {
	mario->changeState(new GrowState(mario));
}