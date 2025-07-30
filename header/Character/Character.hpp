#pragma once
#include <algorithm>
#include "State.hpp"
#include "Block.hpp"
#include <raylib.h>
#include <vector>
using namespace std;

class Character : public GameObject
{
private:
    // Generic friend classes for states
    friend class State;
    friend class EnemyIdleState;
    friend class EnemyWalkState;
    friend class EnemyDeadState;
    friend class EnemyRunState;
    friend class Enemy;
    friend class Goomba;
    friend class Koopa;
    friend class Lakitu;
    friend class PiranhaPlant;
    friend class Spiny;
    friend class Mario;

protected:
    // Common state properties
    float groundPosY = 0;

public:
    bool changeBody = false; // Check when the character's body needs to be changed
    bool isGrounded = false;
    int groundContacts = 0;

public:
    Character() = default;

    // Collision handling
    virtual void updateCollision(GameObject *other, int type) override;

    virtual void createBody(b2World *world) override;

    // Used to change the character's body when the hitbox changes
    virtual void toNewBody();
};
