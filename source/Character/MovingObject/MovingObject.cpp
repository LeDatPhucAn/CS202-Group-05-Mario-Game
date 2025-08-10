#include "MovingObject.hpp"

MovingObject::MovingObject() : Character() {}


MovingObject::MovingObject(const MovingObject &other) : Character(other)
{
    beCleared = other.beCleared;
}

void MovingObject::update(const Vector2& marioPos)
{
    Character::update();
}