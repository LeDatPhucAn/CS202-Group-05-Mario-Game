#pragma once
#include "box2d/box2d.h"
#include <vector>
#include "raylib.h"
#include <algorithm>
#include <cmath>
using namespace std;

enum CollisionType
{
    NONE,
    HEAD,      // A HEAD B là A đứng trên B
    FEET,      // A nhảy lên đụng B
    LEFTSIDE,  // A ở bên trái B
    RIGHTSIDE, // A ở bên phải B
};

// Là một hình chữ nhật, mô tả mọi Object trong Game.
class GameObject
{
public:
    GameObject() = default;
    GameObject(const Vector2 &_position, const Vector2 &size)
        : pos(_position), size(size) {}

    virtual ~GameObject() = default;

    virtual void update() = 0;

    virtual void display() = 0;

    // Xử lý khi va chạm với một object khác
    virtual void updateCollision(GameObject *other, int type)
    {
    }

    // Box2D Integration
    void attachBody(b2Body *b) { body = b; }
    b2Body *getBody() const { return body; }
    virtual void createBody(b2World *world) = 0;

    const Vector2 &getPosition() const { return pos; }
    void setPosition(const Vector2 &_pos) { pos = _pos; }

    const Vector2 &getSize() const { return size; }
    void setSize(const Vector2 &s) { size = s; }

    virtual Rectangle getBounds() const
    {
        return {pos.x, pos.y, size.x, size.y};
    }

    const Vector2 getCenter() { return {pos.x + size.x / 2, pos.y + size.y / 2}; }
    float slice = 2;
    bool isStatic = true;

protected:
    // Góc trái trên của Rec
    Vector2 pos;
    // Size Rectangle
    Vector2 size;

    // Box2D body pointer
    b2Body *body = nullptr;
};
