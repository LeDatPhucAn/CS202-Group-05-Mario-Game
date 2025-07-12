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