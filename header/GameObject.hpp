#pragma once

#include "box2d/box2d.h"
#include <vector>
#include <string>
#include "raylib.h"
#include "Vec2Adapter.hpp"
#include "Structs.hpp"
#include "UI.hpp"

class State;

class GameObject
{
public:
    GameObject();
    GameObject(const Vector2 &_position, const Vector2 &_size);
    virtual ~GameObject();

    virtual void changeState(State *newState);
    virtual void update();
    virtual void display();

    virtual void updateCollision(GameObject *other, int type);
    virtual void createBody(b2World *world);

    void attachBody(b2Body *b);
    b2Body *getBody() const;

    virtual void setTexture(const std::string &name);

    const Vec2Adapter &getPositionAdapter() const;
    void setPositionAdapter(const Vec2Adapter &_pos);

    const Vec2Adapter &getSizeAdapter() const;
    void setSizeAdapter(const Vec2Adapter &_size);

    Vector2 getPosition() const;
    virtual void setPosition(const Vector2 &_pos);

    Vector2 getSize() const;
    void setSize(const Vector2 &_size);

    Direction getDirection() const;
    void setDirection(Direction dir);

    Sprite getSprite() const;
    virtual Rectangle getBounds() const;

    Vector2 getCenter() const;

    float slice = 2;
    bool isStatic = true;

protected:
    Vec2Adapter pos;
    Vec2Adapter size;
    b2Body *body = nullptr;
    Sprite sprite;
    Direction direction = RIGHT;
    State *currentState = nullptr;

private:
    friend class State;
};
