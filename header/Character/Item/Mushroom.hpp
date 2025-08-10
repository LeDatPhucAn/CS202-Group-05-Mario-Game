#pragma once
#include "MovingObject.hpp"
class Mushroom : public MovingObject
{
public:
    Mushroom();

    void updateCollision(GameObject *other, int type) override;
};
