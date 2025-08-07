#pragma once
#include "Projectile.hpp"

class FireBall : public Projectile
{

public:
    FireBall();
    ~FireBall()
    {
        cout << "Destroying FireBall at " << this << "\n";
    }

    void createBody(b2World *world) override;
    void updateCollision(GameObject *other, int type) override;
};