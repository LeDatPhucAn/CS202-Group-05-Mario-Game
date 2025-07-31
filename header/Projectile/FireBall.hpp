#pragma once
#include "Projectile.hpp"

class FireBall : public Projectile
{

public:
    FireBall();
    void createBody(b2World *world) override;
    void updateCollision(GameObject *other, int type) override;
};