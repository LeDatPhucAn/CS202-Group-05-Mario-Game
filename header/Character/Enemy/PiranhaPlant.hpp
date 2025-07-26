#pragma once
#include "Enemy.hpp"

class PiranhaPlant : public Enemy {
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
    
    void update() override;
    void updateCollision(GameObject *other, int type) override;
};