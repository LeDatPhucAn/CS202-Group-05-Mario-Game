#include "BulletBill.hpp"

#include "Mario.hpp"
#include "Game.hpp"
#include <cmath>

BulletBill::BulletBill()
    : MovingObject()
{
    setFrame(movingObjectStateType::IDLE, 1, 1);
    setFrame(movingObjectStateType::FLY, 6, 1);
    setFrame(movingObjectStateType::DEAD, 1, 1);
    this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["BulletBill"]);
    this->sprite.texture = UI::textureMap["BulletBill"];

    this->isActivated = false;
    this->changeState(new MovingObjectIdleState(this));
}

void BulletBill::update(const Vector2 &marioPos)
{
    b2Vec2 vel = this->body->GetLinearVelocity();
    vel.x = this->direction * fabs(100.f / PPM);
    vel.y = -0.73f;
    this->body->SetLinearVelocity(vel);

    // Check distance to Mario and activate if close enough
    if (!isActivated)
    {
        Vector2 bulletPos = this->getPosition();
        float distance = Vector2Distance(marioPos, bulletPos);

        if (distance <= activationDistance)
        {
            // Activate BulletBill
            this->direction = (marioPos.x < bulletPos.x) ? LEFT : RIGHT;
            this->changeState(new MovingObjectFlyState(this));
            this->isActivated = true;
        }
    }

    MovingObject::update(marioPos);
}

void BulletBill::updateCollision(GameObject *other, int type)
{
    Character::updateCollision(other, type);

    // Handle collision with Mario
    Mario *mario = dynamic_cast<Mario *>(other);
    if (mario)
    {
        if (dynamic_cast<DeadState *>(mario->currentState) || dynamic_cast<MovingObjectDeadState *>(this->currentState))
        {
            return;
        }

        // BulletBill can only be defeated by stomping from above
        if (type == TOP)
        {   
            mario->jumpFromEnemy();
            this->changeState(new MovingObjectDeadState(this));
            return;
        }
        else {
            mario->hitByEnemy();
        }
    }

    // BulletBill destroys other enemies on contact (only when activated)
    if (isActivated)
    {
        MovingObject *otherEnemy = dynamic_cast<MovingObject *>(other);
        if (otherEnemy && otherEnemy != this)
        {
            if (!dynamic_cast<MovingObjectDeadState *>(otherEnemy->currentState))
            {
                otherEnemy->changeState(new MovingObjectDeadState(otherEnemy));
            }
        }
        // BulletBill dies when hitting blocks
        Block *block = dynamic_cast<Block *>(other);
        if (block && (type == LEFTSIDE || type == RIGHTSIDE))
        {
            this->changeState(new MovingObjectDeadState(this));
        }
    }
    
}