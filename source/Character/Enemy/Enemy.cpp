#include "Enemy.hpp"

Enemy::Enemy() : Character() {}
Enemy::Enemy(const Sprite &_sprite, State *_initialState, Vector2 _pos)
    : Character(_sprite, _initialState, _pos) {}

Enemy::Enemy(const Enemy &other) : Character(other)
{
    beCleared = other.beCleared;
}

void Enemy::update(const Vector2& marioPos)
{
    Character::update();
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
Enemy::Builder &Enemy::Builder::setState(State *initialState)
{
    state = initialState;
    return *this;
}


