#pragma once
#include "MovingObject.hpp"

class Goomba : public MovingObject
{
public:
    Goomba();

    void updateCollision(GameObject *other, int type) override;
};