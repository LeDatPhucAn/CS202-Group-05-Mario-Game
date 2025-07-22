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
    Enemy(const Sprite &_sprite, State *_initialState, Vector2 _pos);
    Enemy(const Enemy &other);

    virtual void updateCollision(GameObject *other, int type) = 0;
    struct Builder
    {
        Sprite sprite;
        State *state = nullptr;

        Builder &setFrames(stateType type, int start, int end);
        Builder &setJsonAndTexture(const std::string &name);
        Builder &setState(State *initialState);
    };
};