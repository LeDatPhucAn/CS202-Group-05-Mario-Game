#pragma once
#include "MovingObject.hpp"
class FireBall : public MovingObject
{
public:
    FireBall();
    void updateCollision(GameObject *other, int type) override;
};

class LuigiFireBall : public MovingObject
{
public:
    LuigiFireBall();
    void updateCollision(GameObject *other, int type) override;
    void createBody(b2World *world) override;
};