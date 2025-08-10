#pragma once
#include "MovingObject.hpp"

class BulletBill : public MovingObject
{
private:
    float activationDistance = 250.0f; // Distance in pixels to activate
    bool isActivated = false;
    bool targetYSet = false; //giữ cho nó luôn trên 1 đường thẳng
    float targetY;

public:
    BulletBill();

    void update(const Vector2& marioPos) override;
    void updateCollision(GameObject *other, int type) override;
    void display() override; // Add this line
};