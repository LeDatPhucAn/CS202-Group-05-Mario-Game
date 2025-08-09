#pragma once
#include "Enemy.hpp"


class Lakitu : public Enemy
{
private:
    float throwTimer;
    float throwCooldown = 6.0f; // Throws a Spiny every 6 seconds

public:
    Lakitu();

    void update(const Vector2& marioPos) override;
    void updateCollision(GameObject *other, int type) override;
};