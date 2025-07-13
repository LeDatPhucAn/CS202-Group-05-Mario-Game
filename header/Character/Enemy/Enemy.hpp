#pragma once
#include "Character.hpp"
#include "UI.hpp"
#include "MarioState.hpp"
#pragma once
#include "Character.hpp"
#include "UI.hpp"

// Forward declare state classes
class State;
class EnemyState;

class Enemy : public Character
{
public:
    bool beCleared = false;

    Enemy();
    Enemy(const Sprite &_sprite, const Movement &_movement, State *_initialState, Vector2 _pos);
    Enemy(const Enemy &other);

    // Pure virtual function forces Goomba and Koopa to provide their own implementation.
    virtual void updateCollision(GameObject *other, int type) = 0;

public:
    // The Builder is kept for use in Game.cpp
    struct Builder
    {
        Sprite sprite;
        Movement movement;
        State *state = nullptr;
        Vector2 pos = {100, 0};

        Builder &setFrames(stateType type, int start, int end);
        Builder &setJsonAndTexture(const std::string &name);
        Builder &setSpeed(int speed);
        Builder &setPos(Vector2 _pos);
        Builder &setVelocity(Vector2 velocity);
        Builder &setAcceleration(Vector2 acceleration);
        Builder &setState(State *initialState);
        // The build() method is specific to each enemy type, so it's removed from the base.
    };
};

class Goomba : public Enemy
{
public:
    Goomba();
    Goomba(const Builder &builder); // Add a constructor that takes a builder

    void updateCollision(GameObject *other, int type) override;
};

class Koopa : public Enemy
{
public:
    Koopa();
    Koopa(const Builder &builder); // Add a constructor that takes a builder

    void updateCollision(GameObject *other, int type) override;
    Rectangle getFeet() const override
    {
        return Rectangle{
            pos.x + 6,
            pos.y + size.y - 2, // Feet is at the bottom of the sprite
            size.x - 12,
            6.0f};
    }
};