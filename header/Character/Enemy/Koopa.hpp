#pragma once
#include "Enemy.hpp"

class Koopa : public Enemy
{
public:
    Koopa();
    Koopa(const Builder &builder);

    void updateCollision(GameObject *other, int type) override;
};