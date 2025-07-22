#pragma once
#include "box2d/box2d.h"
#include <vector>
#include "raylib.h"
#include <algorithm>
#include <cmath>
#include "Vec2Adapter.hpp"
using namespace std;

enum CollisionType
{
    NONE,
    TOP,       // TOP là đầu của 1 object
    BOTTOM,    // BOTTOM là phần dưới của 1 object
    LEFTSIDE,  // LEFTSIDE là phần bên trái của 1 object
    RIGHTSIDE, // RIGHTSIDE là phần bên phải của 1 object
};

// global values
const float fallGravity = 9.8f;

const float offset = 3;
const float jumpGravity = fallGravity * offset;
const float addedFallGravity = fallGravity * (offset + 0.5);

// jumping
const float maxJumpHeight = 4;
const float jumpVel = -sqrt(2 * (fallGravity + jumpGravity) * maxJumpHeight);
class GameObject
{
public:
    GameObject() = default;

    GameObject(const Vector2 &_position, const Vector2 &_size)
        : pos(_position), size(_size) {}

    virtual ~GameObject() = default;

    virtual void update() {};
    virtual void display() {};

    // Xử lý khi va chạm với một object khác
    virtual void updateCollision(GameObject *other, int type) {}

    // Box2D Integration
    void attachBody(b2Body *b) { body = b; }
    b2Body *getBody() const { return body; }
    virtual void createBody(b2World *world) {};

    // Accessors for position and size
    const Vec2Adapter &getPositionAdapter() const { return pos; }
    void setPositionAdapter(const Vec2Adapter &_pos) { pos = _pos; }

    const Vec2Adapter &getSizeAdapter() const { return size; }
    void setSizeAdapter(const Vec2Adapter &_size) { size = _size; }

    // Convenience: get raw Vector2 in pixels
    Vector2 getPosition() const { return pos.toPixels(); }
    void setPosition(const Vector2 &_pos) { pos = Vec2Adapter(_pos); }

    Vector2 getSize() const { return size.toPixels(); }
    void setSize(const Vector2 &_size) { size = Vec2Adapter(_size); }

    virtual Rectangle getBounds() const
    {
        Vector2 p = pos.toPixels();
        Vector2 s = size.toPixels();
        return {p.x, p.y, s.x, s.y};
    }

    Vector2 getCenter() const
    {
        Vector2 p = pos.toPixels();
        Vector2 s = size.toPixels();
        return {p.x - s.x / 2, p.y - s.y / 2};
    }

    float slice = 2;
    bool isStatic = true;

protected:
    Vec2Adapter pos;  // Top-left corner in pixels
    Vec2Adapter size; // Size in pixels
    b2Body *body = nullptr;
};
