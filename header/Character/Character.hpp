#pragma once

#include <algorithm>
#include "State.hpp"
#include "Block.hpp"
#include <raylib.h>
#include <vector>
#include "UI.hpp"
#include "MarioState.hpp"

using namespace std;

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
    friend class CollisionManager;
    friend class State;
    friend class RunState;
    friend class IdleState;
    friend class WalkState;
    friend class JumpState;
    friend class FallState;
    friend class SkidState;
    friend class CrouchState;
    friend class GrowState;
    friend class UnGrowState;
    MarioForm form = SMALL;
    void changeForm(MarioForm form);

protected:
    const float maxJumpHeight = 72.0f;
    const float jumpGravity = 1056.25f; // While holding jump
    const float fallGravity = 1462.5f;  // Letting go / falling
    const float fallSpeedCap = 240.0f;

    float maxHeight = 72.0f;

protected:
    bool isGrounded = false;
    float groundPosY = 0;
    Direction direction = RIGHT;
    Sprite sprite;
    Movement movement;
    State *currentState = nullptr;

public:
    Character() {}

    Character(const Sprite &_sprite, const Movement &_movement, State *_initialState, Vector2 _pos)
        : GameObject(_pos, {14, 16}), sprite(_sprite), movement(_movement), currentState(_initialState)
    {
    }

    ~Character()
    {
        if (currentState)
            delete currentState;
    }
    Character(const Character &other)
    {
        sprite = other.sprite;
        movement = other.movement;
        isGrounded = other.isGrounded;
        direction = other.direction;
        pos = other.pos;

        currentState = new IdleState(this);
    }

    Character &operator=(const Character &other)
    {
        if (this == &other)
            return *this;
        // Clean up existing state
        if (currentState)
            delete currentState;

        sprite = other.sprite;
        movement = other.movement;
        isGrounded = other.isGrounded;
        direction = other.direction;
        pos = other.pos;

        currentState = new IdleState(this);
        return *this;
    }
    void changeState(State *newState);
    void update() override;
    void display() override;

    // Collision
    void updateCollision(GameObject *other, int type) override;
    Rectangle getBounds() const override;
    Rectangle getActualBounds() const;
    Rectangle getFeet() const override;
    Rectangle getHead() const override;
    Rectangle getLeft() const override;
    Rectangle getRight() const override;

public:
    struct Builder
    {
        Sprite sprite;
        Movement movement;
        State *state = nullptr;
        Vector2 pos = {0, 0};

        // --- Sprite setters ---
        Builder &setFrames(stateType type, int start, int end)
        {
            sprite.StartEndFrames[type] = {start, end};
            return *this;
        }
        Builder &setJsonAndTexture(string name)
        {
            sprite.texture = UI::textureMap[name];
            sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap[name]);
            return *this;
        }

        // --- Movement setters ---
        Builder &setSpeed(int speed)
        {
            movement.speed = speed;
            return *this;
        }

        Builder &setPos(Vector2 _pos)
        {
            pos = _pos;
            return *this;
        }
        Builder &setVelocity(Vector2 velocity)
        {
            movement.velocity = velocity;
            return *this;
        }

        Builder &setAcceleration(Vector2 acceleration)
        {
            movement.acceleration = acceleration;
            return *this;
        }

        // --- State ---
        Builder &setState(State *initialState)
        {
            state = initialState;
            return *this;
        }

        // --- Final build ---
        Character build()
        {
            return Character(sprite, movement, state, pos);
        }
    };
};
