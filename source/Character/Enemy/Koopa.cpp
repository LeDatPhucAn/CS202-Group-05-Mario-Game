#include "Koopa.hpp"
#include "EnemyState.hpp"
#include "Mario.hpp"
#include "Game.hpp"
Koopa::Koopa()
    : Enemy()
{
    setFrame(enemyStateType::IDLE, 7, 7);
    setFrame(enemyStateType::WALK, 3, 4);
    setFrame(enemyStateType::RUN, 7, 7);
    setFrame(enemyStateType::JUMP, 5, 6); 
    setFrame(enemyStateType::DEAD, 7, 7);
    this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Enemies2D"]);
    this->sprite.texture = UI::textureMap["Enemies2D"];
    this->changeState(new EnemyWalkState(this));
}

Koopa::Koopa(bool isFlying)
    : Enemy()
{
    setFrame(enemyStateType::IDLE, 7, 7);
    setFrame(enemyStateType::WALK, 3, 4);
    setFrame(enemyStateType::RUN, 7, 7);
    setFrame(enemyStateType::JUMP, 5, 6); 
    setFrame(enemyStateType::DEAD, 7, 7);
    this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Enemies2D"]);
    this->sprite.texture = UI::textureMap["Enemies2D"];
    
    if (isFlying)
        this->changeState(new EnemyJumpState(this));
    else
        this->changeState(new EnemyWalkState(this));
}

void Koopa::updateCollision(GameObject *other, int type)
{
    Character::updateCollision(other, type);
    Mario *mario = dynamic_cast<Mario *>(other);
    if (mario)
    {
        if (dynamic_cast<DeadState *>(mario->currentState) || dynamic_cast<EnemyDeadState *>(this->currentState))
        {
            return;
        }
        if (type == TOP)
        {   
            // Use a cooldown to prevent multiple hits in one frame
            static float lastHitTime = 0.0f;
            float currentTime = GetTime();
            if (currentTime - lastHitTime < 0.2f)
            {
                return;
            }
            lastHitTime = currentTime;
            
            // Check if it's a Flying Koopa (which starts in JumpState)
            if (dynamic_cast<EnemyJumpState *>(this->currentState))
            {
                // First hit: loses wings and becomes a walking Koopa
                this->changeState(new EnemyWalkState(this));
                return;
            }
            // Second hit: becomes a shell
            else if (dynamic_cast<EnemyWalkState *>(this->currentState))
            {
                this->toNewBody();
                this->changeState(new EnemyIdleState(this));
                return;
            }
            // Third hit: stops the shell if it's running
            else if (dynamic_cast<EnemyRunState *>(this->currentState))
            {
                this->changeState(new EnemyIdleState(this));
                return;
            }
        }
        
        // If Mario hits an idle shell, it starts moving
        if (dynamic_cast<EnemyIdleState *>(this->currentState))
        {
            this->direction = (mario->body->GetPosition().x < this->body->GetPosition().x) ? RIGHT : LEFT;
            this->changeState(new EnemyRunState(this));
            return;
        }
    }

    Block *block = dynamic_cast<Block *>(other);
    Enemy *enemy = dynamic_cast<Enemy *>(other);
    if (block || enemy)
    {
        // If a running shell hits another enemy, the other enemy is defeated
        if (dynamic_cast<EnemyRunState *>(this->currentState) && enemy)
        {
            enemy->changeState(new EnemyDeadState(enemy));
        }
        // Standard wall collision logic
        if (type == LEFTSIDE)
        {
            this->direction = RIGHT;
        }
        else if (type == RIGHTSIDE)
        {
            this->direction = LEFT;
        }
    }
}






