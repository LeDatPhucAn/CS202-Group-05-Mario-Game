#pragma once
#include "MovingObject.hpp"

class Spiny : public MovingObject
{
public:
    Spiny();
    void updateCollision(GameObject *other, int type) override;
};
