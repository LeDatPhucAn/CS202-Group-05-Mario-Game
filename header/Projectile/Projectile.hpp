#pragma once
#include "GameObject.hpp"

class Projectile : public GameObject
{

public:
    Projectile() = default;
    virtual ~Projectile() = default;
    Projectile(Vector2 _pos, Vector2 _size)
        : GameObject(_pos, _size) {}
    void setFrame(projectileStateType type, int start, int end)
    {
        this->sprite.StartEndFrames[(int)type] = {start, end};
    }
};
