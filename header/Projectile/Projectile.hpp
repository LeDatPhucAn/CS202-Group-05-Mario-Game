#pragma once
#include "GameObject.hpp"
#include "box2d/box2d.h"
#include "UI.hpp"
#include "Character.hpp"

class Projectile : public GameObject
{

public:
    Projectile();
    Projectile(Vector2 _pos, Vector2 _size)
        : GameObject(_pos, _size) {}

    void setFrame(projectileStateType type, int start, int end)
    {
        this->sprite.StartEndFrames[(int)type] = {start, end};
    }
};
