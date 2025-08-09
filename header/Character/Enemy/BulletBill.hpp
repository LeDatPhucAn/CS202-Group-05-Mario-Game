#pragma once
#include "Enemy.hpp"

class BulletBill : public Enemy
{
private:
    float activationDistance = 200.0f; // Distance in pixels to activate
    bool isActivated = false;

public:
    BulletBill();

    void update(const Vector2& marioPos) override;
    void updateCollision(GameObject *other, int type) override;
};