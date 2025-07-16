#include "Goomba.hpp"
#include "EnemyState.hpp"
#include "Mario.hpp"

Goomba::Goomba()
    : Enemy()
{
    this->sprite.StartEndFrames[IDLE] = {0, 0};
    this->sprite.StartEndFrames[WALK] = {0, 1};
    this->sprite.StartEndFrames[DEAD] = {2, 2};
    this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Enemies2D"]);
    this->sprite.texture = UI::textureMap["Enemies2D"];
}

void Goomba::updateCollision(GameObject *other, int type)
{
    Character::updateCollision(other, type);
    Block *block = dynamic_cast<Block *>(other);
    if (block)
    {
        if (type == LEFTSIDE){
            this->direction = RIGHT;
            this->movement.velocity.x = -20.f;
        }
        else if (type == RIGHTSIDE){
            this->direction = LEFT;
            this->movement.velocity.x = 20.f;
        }
    }

    Mario *mario = dynamic_cast<Mario *>(other);
    if (mario)
    {
        if (dynamic_cast<DeadState *>(mario->currentState) || dynamic_cast<EnemyDeadState *>(this->currentState))
        {
            return;
        }
        if (type == HEAD)
        {
            this->changeState(new EnemyDeadState(this));
            mario->movement.velocity.y = -200.f;
        }
        return; 
    }
}