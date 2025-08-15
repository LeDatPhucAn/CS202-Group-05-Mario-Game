#pragma once
#include "MovingObject.hpp"

class Goomba : public MovingObject
{
private:
    bool isActivated = false;
    float activationDistance = 300.0f; // pixels
public:
    Goomba();

    void updateCollision(GameObject *other, int type) override;
    void update(const Vector2& marioPos) override;
    void display() override;
};