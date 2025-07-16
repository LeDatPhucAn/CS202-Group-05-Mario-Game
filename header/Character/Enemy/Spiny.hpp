#pragma once
#include "Enemy.hpp"

class Spiny : public Enemy
{
public:
    Spiny();
    void updateCollision(GameObject *other, int type) override;
};

