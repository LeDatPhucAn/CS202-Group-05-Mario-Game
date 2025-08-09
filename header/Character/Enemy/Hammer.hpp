#pragma once
#include "Enemy.hpp"

class Hammer : public Enemy
{
public:
    Hammer();
    void update(const Vector2 &marioPos) override;
    void updateCollision(GameObject *other, int type) override;
};