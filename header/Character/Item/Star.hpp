#pragma once
#include "Enemy.hpp"
class Star : public Enemy
{
public:
    Star();
    void updateCollision(GameObject *other, int type) override;
};
