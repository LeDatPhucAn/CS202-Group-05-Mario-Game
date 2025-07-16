#pragma once
#include "Character.hpp"
#include "UI.hpp"
#include "MarioState.hpp"
#include "EnemyState.hpp"


// Forward declare state classes
class State;

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
    Rectangle getBounds() const
    {
        // If the Koopa is walking, use the taller bounds.
        if (dynamic_cast<EnemyWalkState*>(currentState))
        {
            // These values are examples, adjust them to fit your walking sprite
            return Rectangle{ pos.x, pos.y, 16.0f, 24.0f }; 
        }
        // If the Koopa is a shell (idle or sliding), use the shorter bounds.
        else if (dynamic_cast<EnemyIdleState*>(currentState) || dynamic_cast<EnemyRunState*>(currentState))
        {
            // These values are examples, adjust them to fit your shell sprite
            return Rectangle{ pos.x, pos.y, 16.0f, 16.0f };
        }
        
        // Fallback to the default implementation if state is unknown
        return Character::getBounds();
    }

    Rectangle getFeet() const
    {
        Rectangle bounds = getBounds(); // Use the new state-aware getBounds()
        return Rectangle{
            bounds.x + 4,
            bounds.y + bounds.height - 2, // Feet are now correctly at the bottom of the current bounds
            bounds.width - 8,
            4.0f
        };
    }
};