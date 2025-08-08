#pragma once
#include "Enemy.hpp"
class FireBall : public Enemy
{
public:
    FireBall();

    void updateCollision(GameObject *other, int type) override;
};
