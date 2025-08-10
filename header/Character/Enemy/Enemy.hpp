#pragma once
#include "Character.hpp"
#include "UI.hpp"
#include "playerStatee.hpp"
#include "MovingObjectState.hpp"
#include "Structs.hpp"
class State;

class MovingObject: public Character
{
public:
    bool beCleared = false;

    void setFrame(movingObjectStateType type, int start, int end)
    {
        this->sprite.StartEndFrames[(int)type] = {start, end};
    }
    MovingObject();
    MovingObject(const MovingObject &other);
    virtual void update(const Vector2& marioPos);
    virtual void updateCollision(GameObject *other, int type) = 0;
};
