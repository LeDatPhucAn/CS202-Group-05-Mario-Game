#pragma once
#include "Character.hpp"
#include "UI.hpp"
#include "MarioState.hpp"
#include "EnemyState.hpp"
#include "Structs.hpp"
class State;

class Enemy : public Character
{
public:
    bool beCleared = false;

    void setFrame(enemyStateType type, int start, int end)
    {
        this->sprite.StartEndFrames[(int)type] = {start, end};
    }
    Enemy();
    Enemy(const Enemy &other);

    virtual void updateCollision(GameObject *other, int type) = 0;


};