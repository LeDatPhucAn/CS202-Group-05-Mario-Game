#pragma once
#include "MovingObject.hpp"
class FireBall : public MovingObject
{
public:
    FireBall();
    void updateCollision(GameObject *other, int type) override;
};
