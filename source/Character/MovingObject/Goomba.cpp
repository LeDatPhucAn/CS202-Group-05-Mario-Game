#include "Goomba.hpp"
#include "MovingObjectState.hpp"
#include "Player.hpp"

Goomba::Goomba()
    : MovingObject()
{
    setFrame(movingObjectStateType::IDLE, 2, 2);
    setFrame(movingObjectStateType::WALK, 2, 10);
    setFrame(movingObjectStateType::DEAD, 12, 12);
    this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Goomba"]);
    this->sprite.texture = UI::textureMap["Goomba"];
    this->changeState(new MovingObjectWalkState(this));
}

void Goomba::updateCollision(GameObject *other, int type)
{
    Character::updateCollision(other, type);
    Block *block = dynamic_cast<Block *>(other);
    MovingObject *enemy = dynamic_cast<MovingObject *>(other);
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

    Player *mario = dynamic_cast<Player *>(other);
    if (mario)
    {
        if (dynamic_cast<DeadState *>(mario->currentState) || dynamic_cast<MovingObjectDeadState *>(this->currentState))
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
            this->changeState(new MovingObjectDeadState(this));
            mario->jumpFromEnemy();
        }
        else
        {
            // change mario state accordingly when hit by enemy
            mario->hitByEnemy();
        }
    }
}