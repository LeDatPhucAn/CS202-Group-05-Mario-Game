#pragma once
#include "Enemy.hpp"

class Goomba : public Enemy
{
public:
    Goomba();
    Goomba(const Builder &builder);

    void updateCollision(GameObject *other, int type) override;
};