#pragma once
#include "GameObject.hpp"

class Projectile : public GameObject
{

public:
    Projectile() : GameObject() {};
    
    void setFrame(projectileStateType type, int start, int end)
    {
        this->sprite.StartEndFrames[(int)type] = {start, end};
    }
};
