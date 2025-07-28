#pragma once
#include "Enemy.hpp"

class Koopa : public Enemy
{
public:
    Koopa();

    void updateCollision(GameObject *other, int type) override;
};
