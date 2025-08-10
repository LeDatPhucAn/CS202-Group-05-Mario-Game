#pragma once
#include "MovingObject.hpp"

class Hammer : public MovingObject
{
public:
    Hammer();
    void update(const Vector2 &marioPos) override;
    void updateCollision(GameObject *other, int type) override;
};