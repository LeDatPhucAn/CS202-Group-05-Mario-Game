#pragma once
#include "Enemy.hpp"

class Goomba : public Enemy
{
public:
    Goomba();

    void updateCollision(GameObject *other, int type) override;
};