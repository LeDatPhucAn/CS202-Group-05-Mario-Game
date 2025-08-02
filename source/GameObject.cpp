#include "GameObject.hpp"
#include "State.hpp"
#include "Game.hpp"
GameObject::GameObject() = default;

GameObject::GameObject(const Vector2 &_position, const Vector2 &_size)
    : pos(_position), size(_size) {}

GameObject::~GameObject()
{
    if (currentState)
    {
        currentState->gameObject = nullptr;
        delete currentState;
        currentState = nullptr;
    }
    if (body)
    {
        Game::world->DestroyBody(body);
        body = nullptr;
    }
}

void GameObject::changeState(State *newState)
{
    if (currentState)
        delete currentState;
    currentState = newState;
}

void GameObject::update()
{
    if (currentState)
    {
        currentState->updateState();
    }
}

void GameObject::display()
{
    if (currentState)
    {
        currentState->displayState();
    }
}

void GameObject::updateCollision(GameObject *other, int type)
{
    // Default implementation: do nothing
}

void GameObject::createBody(b2World *world)
{
    // Default implementation: do nothing
}

void GameObject::attachBody(b2Body *b)
{
    body = b;
}

b2Body *GameObject::getBody() const
{
    return body;
}

void GameObject::setTexture(const std::string &name)
{
    sprite.texture = UI::textureMap[name];
    sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap[name]);
}

const Vec2Adapter &GameObject::getPositionAdapter() const
{
    return pos;
}

void GameObject::setPositionAdapter(const Vec2Adapter &_pos)
{
    pos = _pos;
}

const Vec2Adapter &GameObject::getSizeAdapter() const
{
    return size;
}

void GameObject::setSizeAdapter(const Vec2Adapter &_size)
{
    size = _size;
}

Vector2 GameObject::getPosition() const
{
    return pos.toPixels();
}

void GameObject::setPosition(const Vector2 &_pos)
{
    pos = Vec2Adapter(_pos);
}

Vector2 GameObject::getSize() const
{
    return size.toPixels();
}

void GameObject::setSize(const Vector2 &_size)
{
    size = Vec2Adapter(_size);
}

Direction GameObject::getDirection() const
{
    return direction;
}

void GameObject::setDirection(Direction dir)
{
    direction = dir;
}

Sprite GameObject::getSprite() const
{
    return sprite;
}

Rectangle GameObject::getBounds() const
{
    Vector2 p = pos.toPixels();
    Vector2 s = size.toPixels();
    return {p.x, p.y, s.x, s.y};
}

Vector2 GameObject::getCenter() const
{
    Vector2 p = pos.toPixels();
    Vector2 s = size.toPixels();
    return {p.x - s.x / 2, p.y - s.y / 2};
}
