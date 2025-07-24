#pragma once
#include "Enemy.hpp"


class Lakitu : public Enemy
{
private:
    float throwTimer;
    float throwCooldown = 4.0f; // Throws a Spiny every 4 seconds

public:
    Lakitu();

    void update(const Vector2& marioPos) override;
    void updateCollision(GameObject *other, int type) override;
};