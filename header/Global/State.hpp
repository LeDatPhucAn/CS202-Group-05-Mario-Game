#pragma once
#include "raylib.h"
#include "raymath.h"
#include "Vec2Adapter.hpp"
#include "Structs.hpp"
class GameObject;
class State
{
private:
    friend class Mario;
    friend class Character;
    friend class Projectile;

protected:
    int numFrames = 0;
    int frameIndex = 0;
    int delay = 0;
    int delayCounter = 0;
    Rectangle frameRec;
    int type;
    GameObject *gameObject = nullptr;

public:
    State();
    State(int _type, GameObject *_gameObject, int _delay);
    void setObjNull()
    {
        gameObject = nullptr;
    }
    virtual ~State();
    virtual int getType() const { return type; }
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
