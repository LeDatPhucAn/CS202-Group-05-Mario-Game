#pragma once
#include "MovingObject.hpp"

class Koopa : public MovingObject
{
public:
    Koopa();
    Koopa(bool isFlying); // Constructor for flying variant

    void updateCollision(GameObject *other, int type) override;
};
