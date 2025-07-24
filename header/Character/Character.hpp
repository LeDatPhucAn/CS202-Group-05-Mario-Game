#pragma once
#include <algorithm>
#include "State.hpp"
#include "Block.hpp"
#include <raylib.h>
#include <vector>
using namespace std;

// These structs are generic and can stay here
struct StartEndFrame
{
    int start = 0;
    int end = 0;
};
struct Sprite
{
    vector<StartEndFrame> StartEndFrames = vector<StartEndFrame>(20);
    vector<Rectangle> frameRecs;
    Texture2D texture;
};

class Character : public GameObject
{
private:
    // Generic friend classes for states
    friend class State;
    friend class EnemyIdleState;
    friend class EnemyWalkState;
    friend class EnemyDeadState;
    friend class EnemyRunState;
    friend class EnemyFlyState;
    friend class Enemy;
    friend class Goomba;
    friend class Koopa;
    friend class flyingKoopa;
    friend class Lakitu;
    friend class PiranhaPlant;
    friend class Spiny;
    friend class BulletBill;
    friend class Mario;

protected:
    // Common state properties
    float groundPosY = 0;
    Direction direction = RIGHT;
    Sprite sprite;
    State *currentState = nullptr;

public:
    bool changeBody = false; // Check when the character's body needs to be changed
    bool isGrounded = false;
    int groundContacts = 0;

public:
    Character() = default;
    Character(const Sprite &_sprite, State *_initialState, Vector2 _pos)
        : GameObject(_pos, {16, 16}), sprite(_sprite), currentState(_initialState) {}

    virtual ~Character()
    {
        if (currentState)
            delete currentState;
    }

    // Virtual methods to be implemented by derived classes
    virtual void changeState(State *newState);
    virtual void update() override;
    virtual void display() override;

    // Collision handling
    virtual void updateCollision(GameObject *other, int type) override;

    virtual void createBody(b2World *world) override;

    // Used to change the character's body when the hitbox changes
    virtual void toNewBody();
};
