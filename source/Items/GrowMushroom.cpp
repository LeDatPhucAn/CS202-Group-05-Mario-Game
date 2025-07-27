#include "GrowMushroom.hpp"

GrowMushroom::GrowMushroom() {
	this->sprite.StartEndFrames[IDLE] = {0, 0};
	this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Items"]);
	this->sprite.texture = UI::textureMap["Items"];
}

void GrowMushroom::affect(Mario* mario) {
	mario->changeState(new GrowState(mario));
}