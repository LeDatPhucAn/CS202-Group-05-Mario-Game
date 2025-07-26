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
            b2Vec2 vel = this->body->GetLinearVelocity();
            vel.x = this->direction * fabs(20 / PPM); // Ensure correct direction
            this->body->SetLinearVelocity(vel);
        }
        else if (type == RIGHTSIDE){
            this->direction = LEFT;
            b2Vec2 vel = this->body->GetLinearVelocity();
            vel.x = this->direction * fabs(20 / PPM); // Ensure correct direction
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