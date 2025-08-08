#pragma once
#include "Enemy.hpp"
class Mushroom : public Enemy
{
public:
    Mushroom();

    void updateCollision(GameObject *other, int type) override;
};
