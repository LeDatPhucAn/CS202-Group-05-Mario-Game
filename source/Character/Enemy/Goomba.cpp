#include "Goomba.hpp"
#include "EnemyState.hpp"
#include "Mario.hpp"

Goomba::Goomba()
    : Enemy()
{
    setFrame(enemyStateType::IDLE, 2, 2);
    setFrame(enemyStateType::WALK, 2, 10);
    setFrame(enemyStateType::DEAD, 12, 12);
    this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Goomba2"]);
    this->sprite.texture = UI::textureMap["Goomba2"];
    this->changeState(new EnemyWalkState(this));
}

void Goomba::updateCollision(GameObject *other, int type)
{
    Character::updateCollision(other, type);
    Block *block = dynamic_cast<Block *>(other);
    Enemy *enemy = dynamic_cast<Enemy *>(other);
    if (block || enemy)
    {
        if (type == LEFTSIDE)
        {
            this->direction = LEFT;
        }
        else if (type == RIGHTSIDE)
        {
            this->direction = RIGHT;
        }
    }

    Mario *mario = dynamic_cast<Mario *>(other);
    if (mario)
    {
        if (dynamic_cast<DeadState *>(mario->currentState) || dynamic_cast<EnemyDeadState *>(this->currentState))
        {
            return;
        }
        if (type == TOP)
        {
            this->changeState(new EnemyDeadState(this));
        }
        return; 
    }
}