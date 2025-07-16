#include "Koopa.hpp"
#include "EnemyState.hpp"
#include "Mario.hpp"

Koopa::Koopa()
    : Enemy()
{
    this->sprite.StartEndFrames[IDLE] = {7, 7};
    this->sprite.StartEndFrames[WALK] = {3, 4};
    this->sprite.StartEndFrames[RUN] = {7, 7};
    this->sprite.StartEndFrames[DEAD] = {7, 7};
    this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Enemies2D"]);
    this->sprite.texture = UI::textureMap["Enemies2D"];
}

void Koopa::updateCollision(GameObject *other, int type)
{
    Character::updateCollision(other, type);
    Block *block = dynamic_cast<Block *>(other);
    if (block)
    {
        if (type == LEFTSIDE){
            this->direction = RIGHT;
            this->movement.velocity.x = -this->movement.velocity.x; 
        }
        else if (type == RIGHTSIDE){
            this->direction = LEFT;
            this->movement.velocity.x = -this->movement.velocity.x;
        }
    }
    Mario *mario = dynamic_cast<Mario *>(other);
    if (mario)
    {
        if (dynamic_cast<DeadState *>(mario->currentState) || dynamic_cast<EnemyDeadState *>(this->currentState))
        {
            return;
        }
        if (dynamic_cast<EnemyWalkState *>(this->currentState))
        {
            if (type == HEAD)
            {                                                
                this->changeState(new EnemyIdleState(this));
                this->pos.y += 8.0f;
            }
            return;
        }
        if (dynamic_cast<EnemyIdleState *>(this->currentState))
        {   
            this->direction = (mario->pos.x < this->pos.x) ? LEFT : RIGHT;
            this->changeState(new EnemyRunState(this)); 
            return;
        }
        if (dynamic_cast<EnemyRunState *>(this->currentState))
        {   
            if (type == HEAD)
            { 
                this->changeState(new EnemyIdleState(this));
            }         
            return;
        }
    }
    Enemy *otherEnemy = dynamic_cast<Enemy *>(other);
    if (otherEnemy && otherEnemy != this)
    {
        if (dynamic_cast<DeadState *>(otherEnemy->currentState) || dynamic_cast<EnemyDeadState *>(this->currentState))
        {
            return;
        }
        if (dynamic_cast<EnemyRunState *>(this->currentState))
        {
            otherEnemy->changeState(new EnemyDeadState(otherEnemy));
        }
    }
}

Rectangle Koopa::getBounds() const
{
    if (dynamic_cast<const EnemyWalkState*>(currentState))
    {
        return { pos.x, pos.y, 16.0f, 24.0f }; 
    }
    else
    {
        return { pos.x, pos.y, 16.0f, 12.0f };
    }
}

Rectangle Koopa::getFeet() const
{
    Rectangle currentBounds = getBounds(); 
    return {
        currentBounds.x + 6, 
        currentBounds.y + currentBounds.height - 2,
        currentBounds.width - 12,
        6.0f
    };
}