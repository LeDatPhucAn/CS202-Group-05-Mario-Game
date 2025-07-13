#include "Items.hpp"

Sprite Items::getSprite() {
	return sprite;
}

Movement Items::getMovement() {
	return movement;
}

Direction Items::getDirection() {
	return direction;
}

void Items::setSprite(Sprite sprite) {
	this->sprite = sprite;
}

void Items::setMovement(Movement movement) {
	this->movement = movement;
}

void Items::setDirection(Direction direction) {
	this->direction = direction;
}

void Items::appear(float dt) {
	// TODO: animate the item moving into view from the block beneath it
	DrawRectangleLines(
		owner->getPosition().x,
	 	owner->getPosition().y - 1,
		sprite.frameRecs[0].width,
		sprite.frameRecs[0].height,
		{168, 168, 0, 255}
	);
	DrawTextureRec(sprite.texture, sprite.frameRecs[0], pos, WHITE);
}

void Items::display() {
	DrawRectangleLines(
		owner->getPosition().x,
	 	owner->getPosition().y - 1,
		sprite.frameRecs[0].width,
		sprite.frameRecs[0].height,
		{168, 168, 0, 255}
	);
	DrawTextureRec(sprite.texture, sprite.frameRecs[0], pos, WHITE);
}