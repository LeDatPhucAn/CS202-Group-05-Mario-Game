#include "Spiny.hpp"
#include "EnemyState.hpp"
#include "Mario.hpp"

Spiny::Spiny()
    : Enemy()
{
    setFrame(enemyStateType::IDLE, 131, 131);
    setFrame(enemyStateType::WALK, 131, 132);
    setFrame(enemyStateType::DEAD, 131, 131);
    setFrame(enemyStateType::FALL, 129, 130);
    this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Enemies2D"]);
    this->sprite.texture = UI::textureMap["Enemies2D"];
}

void Spiny::updateCollision(GameObject *other, int type)
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

    // --- Mario Collision Logic (Different from Goomba) ---
    Mario *mario = dynamic_cast<Mario *>(other);
    if (mario)
    {
        if (dynamic_cast<DeadState *>(mario->currentState) || dynamic_cast<EnemyDeadState *>(this->currentState))
        {
            return;
        }

        // A Spiny hurts Mario even if he stomps on it.
        mario->hitByEnemy();
    }
}