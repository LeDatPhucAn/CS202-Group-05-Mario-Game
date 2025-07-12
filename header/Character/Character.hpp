#pragma once
#include <algorithm>
#include "State.hpp"
#include "Block.hpp"
#include <raylib.h>
#include <vector>
#include "UI.hpp"

using namespace std;

// These structs are generic and can stay here
struct StartEndFrame
{
    int start = 0;
    int end = 0;
};
struct Sprite
{
    vector<StartEndFrame> StartEndFrames = vector<StartEndFrame>(15);
    vector<Rectangle> frameRecs;
    Texture2D texture;
};

struct Movement
{
    int speed = 0;
    Vector2 velocity = {0, 0};
    Vector2 acceleration = {0, 0};
};

class Character : public GameObject
{
private:
    // Generic friend classes for states
    friend class State;
    friend class IdleState;
    friend class WalkState;
    friend class JumpState;
    friend class FallState;
    friend class DeadState;
    friend class EnemyIdleState;
    friend class EnemyWalkState;
    friend class EnemyDeadState;
    friend class EnemyRunState;
    friend class Enemy;
    friend class Goomba;
    friend class Koopa;

protected:
    // Common physics properties
    const float maxJumpHeight = 72.0f;
    const float jumpGravity = 1056.25f;
    const float fallGravity = 1462.5f;
    const float fallSpeedCap = 240.0f;
    float maxHeight = 72.0f;

    // Common state properties
    bool isGrounded = false;
    float groundPosY = 0;
    Direction direction = RIGHT;
    Sprite sprite;
    Movement movement;
    State *currentState = nullptr;

public:
    Character() = default;
    Character(const Sprite &_sprite, const Movement &_movement, State *_initialState, Vector2 _pos)
        : GameObject(_pos, {14, 16}), sprite(_sprite), movement(_movement), currentState(_initialState) {}

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
    virtual Rectangle getBounds() const override;
    virtual Rectangle getActualBounds() const;
    virtual Rectangle getFeet() const override;
    virtual Rectangle getHead() const override;
    virtual Rectangle getLeft() const override;
    virtual Rectangle getRight() const override;
};
