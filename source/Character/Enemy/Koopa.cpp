#include "Koopa.hpp"
#include "EnemyState.hpp"
#include "Mario.hpp"
#include "Game.hpp"
Koopa::Koopa()
    : Enemy()
{
    this->sprite.StartEndFrames[IDLE] = {7, 7};
    this->sprite.StartEndFrames[WALK] = {3, 4};
    this->sprite.StartEndFrames[RUN] = {7, 7};
    this->sprite.StartEndFrames[FLY] = {5, 6}; 
    this->sprite.StartEndFrames[DEAD] = {7, 7};
    this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Enemies2D"]);
    this->sprite.texture = UI::textureMap["Enemies2D"];
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
            static float lastHitTime = 0.0f;
            float currentTime = GetTime(); // Raylib function to get current time
            
            if (currentTime - lastHitTime < 0.2f) // 0.2 second cooldown
            {
                return; // Ignore this hit if too soon after last hit
            }
            lastHitTime = currentTime;
            
            // Check if still flying (has wings)
            if (dynamic_cast<EnemyFlyState *>(this->currentState))
            {
                this->changeState(new EnemyWalkState(this));
                return;
            }
            // Second hit: Become shell
            else if (dynamic_cast<EnemyWalkState *>(this->currentState))
            {
                this->toNewBody();
                this->changeState(new EnemyIdleState(this));
                return;
            }
            // Third hit: Stop shell
            else if (dynamic_cast<EnemyRunState *>(this->currentState))
            {
                this->changeState(new EnemyIdleState(this));
                return;
            }
        }
        
        if (dynamic_cast<EnemyIdleState *>(this->currentState))
        {
            this->direction = (mario->body->GetPosition().x < this->body->GetPosition().x) ? LEFT : RIGHT;
            this->changeState(new EnemyRunState(this));
            return;
        }
    }

    Block *block = dynamic_cast<Block *>(other);
    Enemy *enemy = dynamic_cast<Enemy *>(other);
    if (block || enemy)
    {
        if (dynamic_cast<EnemyRunState *>(this->currentState) && enemy)
        {
            enemy->changeState(new EnemyDeadState(enemy));
        }
        if (type == LEFTSIDE)
        {
            this->direction = RIGHT;
            b2Vec2 vel = this->body->GetLinearVelocity();
            vel.x = this->direction * fabs(vel.x / PPM);
            this->body->SetLinearVelocity(vel);
        }
        else if (type == RIGHTSIDE)
        {
            this->direction = LEFT;
            b2Vec2 vel = this->body->GetLinearVelocity();
            vel.x = this->direction * fabs(vel.x / PPM);
            this->body->SetLinearVelocity(vel);
        }
    }
}






