#pragma once
#include "Structs.hpp"
class Character;
class CollisionManager;
class State
{

protected:
    // Thông số Mario
    const float jumpGravity = 1056.25f; // While holding jump
    const float fallGravity = 1462.5f;  // Letting go / falling


    // Global
    const float fallSpeedCap = 240.0f;


    // basic parameters
    int numFrames = 0;
    int frameIndex = 0;
    int delay = 0;
    int delayCounter = 0;
    stateType type;
    Rectangle frameRec;
    Character *character = nullptr;

public:
    friend class CollisionManager;
    State();
    State(stateType Type, Character *_character, int _delay);

    // collision
    virtual Rectangle Bounds() const;
    virtual Rectangle ActualBounds() const;
    virtual Rectangle Feet() const;
    virtual Rectangle Head() const;
    virtual Rectangle LeftSide() const;
    virtual Rectangle RightSide() const;
    // salient
    virtual void updateState();
    virtual void displayState();

    // Đối với Enemy thì Input là Map collision
    // Đối với Mario thì Input từ bàn phím
    virtual void handleInput() = 0;

    // constantly changing frames to run sprite
    virtual void animate();

    // manage gravity and movements
    virtual void applyPhysics(float deltaTime);
};
