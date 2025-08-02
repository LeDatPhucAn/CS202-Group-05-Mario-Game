#pragma once
#include "Enemy.hpp"

class Hammer : public Enemy
{
private:
    float lifetime = 3.0f; // Hammer disappears after 3 seconds
    float age = 0.0f;

public:
    Hammer();
    void update(const Vector2& marioPos) override;
    void updateCollision(GameObject *other, int type) override;
    // Hammers are simple projectiles and don't need a custom createBody
};

class HammerBro : public Enemy
{
private:
    float throwTimer = 0.0f;
    float throwCooldown = 2.5f; // Throws a hammer every 2.5 seconds

    float jumpTimer = 0.0f;
    float jumpCooldown = 3.0f; // Jumps every 3 seconds
    float jumpForce = 6.0f;

public:
    HammerBro();

    void update(const Vector2& marioPos) override;
    void updateCollision(GameObject *other, int type) override;
};