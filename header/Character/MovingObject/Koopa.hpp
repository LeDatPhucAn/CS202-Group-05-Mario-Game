#pragma once
#include "MovingObject.hpp"

class Koopa : public MovingObject
{
private:
    bool isActivated = false;
    float activationDistance = 300.0f; // pixels
public:
    Koopa();
    Koopa(bool isFlying); // Constructor for flying variant

    void updateCollision(GameObject *other, int type) override;
    void update(const Vector2& marioPos) override;
    void display() override;
};
