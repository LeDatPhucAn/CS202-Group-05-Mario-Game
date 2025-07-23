#pragma once
#include "Enemy.hpp"

// Forward declare Mario to avoid circular dependency
class Mario;
class Game;

class Lakitu : public Enemy
{
private:
    Mario* target = nullptr; // Lakitu needs a reference to Mario
    Game* game = nullptr; // Reference to the game for spawning Spiny
    float throwTimer;
    float throwCooldown = 4.0f; // Throws a Spiny every 4 seconds

public:
    Lakitu();

    void setTarget(Mario* marioTarget, Game* game);

    // Lakitu has custom movement, so we override update()
    void update() override;
    void updateCollision(GameObject *other, int type) override;
};