#pragma once
#include "GameObject.hpp"
#include "box2d/box2d.h"
#include "UI.hpp"
#include "Character.hpp"

class Projectile : public GameObject
{
protected:

public:
    Projectile();
    Projectile(Vector2 _pos, Vector2 _size)
        : GameObject(_pos, _size) {}

    virtual ~Projectile() = default;

    virtual void update() override;
    virtual void display() override;

    virtual void createBody(b2World *world) override;
    // Collision handling
    virtual void updateCollision(GameObject *other, int type) override;
};
