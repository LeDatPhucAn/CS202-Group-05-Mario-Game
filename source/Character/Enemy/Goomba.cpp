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
        static float lastHitTime = 0.0f;
        float currentTime = GetTime();
        if (currentTime - lastHitTime < 0.2f)
        {
            return;
        }
        lastHitTime = currentTime;
        // Only apply timing cooldown for side collisions (damage), not top collisions (stomp)
        if (type == TOP)
        {
            this->changeState(new EnemyDeadState(this));
            float mass = mario->body->GetMass();
            b2Vec2 impulse(0, mass * jumpVel / 1.2f);
            mario->body->ApplyLinearImpulseToCenter(impulse, true);
        }
    }
}