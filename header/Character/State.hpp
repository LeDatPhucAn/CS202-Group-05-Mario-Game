#pragma once
#include "raylib.h"
#include "raymath.h"
#include "Vec2Adapter.hpp"
#include "GameObject.hpp"

enum stateType
{
    IDLE,
    WALK,
    RUN,
    JUMP,
    FALL,
    SKID,
    CROUCH,
    GROW,
    UNGROW,
    DEAD
};

/*

    if (animation.any())
    {
        auto frames = animation.getFrames();

        int index = getIndex();
        this->sprite.StartEndFrames[type] = {index, index + frames.size() - 1};
    }
*/

class State
{
private:
    friend class Mario;
    friend class Character;

protected:
    int numFrames = 0;
    int frameIndex = 0;
    int delay = 0;
    int delayCounter = 0;
    Rectangle frameRec;
    stateType type;
    GameObject *gameObject = nullptr;

public:
    State();
    State(stateType _type, GameObject *_gameObject, int _delay);

    virtual stateType getType() const { return type; }
    // salient
    virtual void updateState();
    virtual void displayState();

    void gameObjectDebug(GameObject *gameObject);
    void drawDebug(GameObject *gameObject);
    // Đối với Enemy thì Input là Map collision
    // Đối với Mario thì Input từ bàn phím
    virtual void handleInput() = 0;

    // constantly changing frames to run sprite
    virtual void animate();
};
