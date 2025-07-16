#pragma once
#include "Character.hpp"
#include "UI.hpp"
#include "MarioState.hpp"
#include "EnemyState.hpp"

class State;

class Enemy : public Character
{
public:
    bool beCleared = false;

    Enemy();
    Enemy(const Sprite &_sprite, const Movement &_movement, State *_initialState, Vector2 _pos);
    Enemy(const Enemy &other);

    virtual void updateCollision(GameObject *other, int type) = 0;

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
    };
};