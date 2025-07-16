#include "PiranhaPlant.hpp"
#include "EnemyState.hpp"
#include "Mario.hpp"

#include "PiranhaPlant.hpp"
#include "EnemyState.hpp"
#include "Mario.hpp"

PiranhaPlant::PiranhaPlant()
    : Enemy()
{
    this->sprite.StartEndFrames[IDLE] = {9, 10}; 
    this->sprite.StartEndFrames[DEAD] = {9, 10};
    this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Enemies2D"]);
    this->sprite.texture = UI::textureMap["Enemies2D"];

    // Piranha Plants do not move via physics
    this->movement.velocity = {0, 0};
    this->movement.acceleration = {0, 0};
    
    // Defer position-based setup to the first update call
    this->timer = 0.0f;
    this->isHiding = true; // Start hidden inside the pipe
    this->isSetup = false;
}

void PiranhaPlant::update()
{
    // One-time setup on the first frame to capture the correct positions
    if (!isSetup)
    {
        
        this->emergedPos = this->pos; 
        this->hiddenPos = { this->pos.x, this->pos.y + 24.0f };
        this->pos = this->hiddenPos;
        isSetup = true;
    }

    Character::update();

    timer += GetFrameTime();

    if (isHiding && timer > hideDuration)
    {
        isHiding = false; // Time to emerge
        timer = 0.0f;
    }
    else if (!isHiding && timer > emergeDuration)
    {
        isHiding = true; // Time to hide
        timer = 0.0f;
    }

    Vector2 targetPos = isHiding ? hiddenPos : emergedPos;
    
    this->pos.y += (targetPos.y - this->pos.y) * 0.1f;
}

void PiranhaPlant::updateCollision(GameObject *other, int type)
{
    // Piranha Plants do not react to blocks or other enemies
    if (dynamic_cast<Block*>(other) || (dynamic_cast<Enemy*>(other) && other != this))
    {
        return;
    }

    Mario *mario = dynamic_cast<Mario *>(other);
    if (mario)
    {
        if (dynamic_cast<DeadState *>(mario->currentState) || dynamic_cast<EnemyDeadState *>(this->currentState))
        {
            return;
        }

        mario->changeState(new DeadState(mario));
    }
}