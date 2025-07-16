#include "Enemy.hpp"

Enemy::Enemy() : Character() {}
Enemy::Enemy(const Sprite &_sprite, const Movement &_movement, State *_initialState, Vector2 _pos)
    : Character(_sprite, _movement, _initialState, _pos) {}

Enemy::Enemy(const Enemy &other) : Character(other)
{
    beCleared = other.beCleared;
}

// Builder methods...
Enemy::Builder &Enemy::Builder::setFrames(stateType type, int start, int end)
{
    sprite.StartEndFrames[type] = {start, end};
    return *this;
}
Enemy::Builder &Enemy::Builder::setJsonAndTexture(const std::string &name)
{
    sprite.texture = UI::textureMap[name];
    sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap[name]);
    return *this;
}
Enemy::Builder &Enemy::Builder::setSpeed(int speed)
{
    movement.speed = speed;
    return *this;
}
Enemy::Builder &Enemy::Builder::setPos(Vector2 _pos)
{
    pos = _pos;
    return *this;
}
Enemy::Builder &Enemy::Builder::setVelocity(Vector2 velocity)
{
    movement.velocity = velocity;
    return *this;
}
Enemy::Builder &Enemy::Builder::setAcceleration(Vector2 acceleration)
{
    movement.acceleration = acceleration;
    return *this;
}
Enemy::Builder &Enemy::Builder::setState(State *initialState)
{
    state = initialState;
    return *this;
}