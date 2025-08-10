#pragma once
#include "MovingObject.hpp"
class Star : public MovingObject
{
public:
    Star();
    void updateCollision(GameObject *other, int type) override;
};
