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
    Mario *mario = dynamic_cast<Mario *>(other);
    if (mario)
    {
        if (dynamic_cast<DeadState *>(mario->currentState) || dynamic_cast<EnemyDeadState *>(this->currentState))
        {
            return;
        }
        if (dynamic_cast<EnemyWalkState *>(this->currentState))
        {
            if (type == TOP)
            {
                this->changeState(new EnemyIdleState(this));
            }
            return;
        }
        if (dynamic_cast<EnemyIdleState *>(this->currentState))
        {
            this->direction = (mario->body->GetPosition().x < this->body->GetPosition().x) ? LEFT : RIGHT;
            this->changeState(new EnemyRunState(this));
            return;
        }
        if (dynamic_cast<EnemyRunState *>(this->currentState))
        {
            if (type == TOP)
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
