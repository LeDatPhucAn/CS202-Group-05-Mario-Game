#pragma once
#include "Enemy.hpp"

class Koopa : public Enemy
{
public:
    Koopa();
    Koopa(bool isFlying); // Constructor for flying variant

    void updateCollision(GameObject *other, int type) override;
};
