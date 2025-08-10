#pragma once
#include "MovingObject.hpp"

class PiranhaPlant : public MovingObject {
private:
    Vector2 emergedPos;
    Vector2 hiddenPos;  
    float timer;
    bool isHiding;
    bool isSetup = false; 
    float hideDuration = 2.0f;
    float emergeDuration = 3.0f;
    friend class Game;

public:
    PiranhaPlant();

    void update(const Vector2 &marioPos) override;
    void updateCollision(GameObject *other, int type) override;
};