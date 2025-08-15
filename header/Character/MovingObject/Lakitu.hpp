#pragma once
#include "MovingObject.hpp"
#include "MovingObjectState.hpp"

class Lakitu : public MovingObject
{
private:
    float throwTimer;
    float throwCooldown = 6.0f; // Throws a Spiny every 6 seconds
    bool isActivated = false;
    float activationDistance = 300.0f; // pixels

public:
    Lakitu();
    void update(const Vector2& marioPos) override;
    void updateCollision(GameObject *other, int type) override;
    void display() override;
    
    friend class LakituThrowState; // Allow throw state to access Lakitu's members
};
class LakituThrowState : public MovingObjectThrowState
{
private:
    Lakitu* lakitu;

public:
    LakituThrowState(Lakitu* character);
    void executeThrow() override;
};

