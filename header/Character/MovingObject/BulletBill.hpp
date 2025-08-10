#pragma once
#include "MovingObject.hpp"

class BulletBill : public MovingObject
{
private:
    float activationDistance = 200.0f; // Distance in pixels to activate
    bool isActivated = false;

public:
    BulletBill();

    void update(const Vector2& marioPos) override;
    void updateCollision(GameObject *other, int type) override;
};