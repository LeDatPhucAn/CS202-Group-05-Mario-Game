#pragma once
#include "Enemy.hpp"

class HammerBro : public Enemy
{
public:
    HammerBro();
    void update(const Vector2 &marioPos) override;
    void updateCollision(GameObject *other, int type) override;

private:
    bool  isOnGround = false;
    float lastJumpTime = 0.0f;
    float jumpInterval = 1.2f;   // seconds between jumps
    float jumpImpulse = 3.8f;    // upward impulse strength (scaled by mass)
    float lastThrowTime = 0.0f;
    float throwInterval = 2.0f;
};