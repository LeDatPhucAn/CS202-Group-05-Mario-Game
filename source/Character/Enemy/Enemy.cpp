#include "Enemy.hpp"

Enemy::Enemy() : Character() {}
// Enemy::Enemy(const Sprite &_sprite, State *_initialState, Vector2 _pos)
    // : Character(_sprite, _initialState, _pos) {}

Enemy::Enemy(const Enemy &other) : Character(other)
{
    beCleared = other.beCleared;
}
