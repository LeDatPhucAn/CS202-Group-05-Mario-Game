#include "Enemy.hpp"

Enemy::Enemy() : Character() {}


Enemy::Enemy(const Enemy &other) : Character(other)
{
    beCleared = other.beCleared;
}

void Enemy::update(const Vector2& marioPos)
{
    Character::update();
}