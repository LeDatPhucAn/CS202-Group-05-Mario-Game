#include "Goomba.hpp"
#include "EnemyState.hpp"
#include "Mario.hpp"

Goomba::Goomba()
    : Enemy()
{
    setFrame(enemyStateType::IDLE, 2, 2);
    setFrame(enemyStateType::WALK, 2, 10);
    setFrame(enemyStateType::DEAD, 12, 12);
    this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Goomba"]);
    this->sprite.texture = UI::textureMap["Goomba"];
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
        if (type == TOP)
        {
            this->changeState(new EnemyDeadState(this));
        }
        return; 
    }
}