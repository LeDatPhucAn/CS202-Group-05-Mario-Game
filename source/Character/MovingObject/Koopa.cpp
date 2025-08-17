#include "Koopa.hpp"
#include "MovingObjectState.hpp"
#include "Player.hpp"
#include "Game.hpp"
#include "SoundController.hpp"
#include "Mushroom.hpp"
#include "Star.hpp"
#include "GameInfo.hpp"
Koopa::Koopa()
    : MovingObject()
{
    setFrame(movingObjectStateType::IDLE, 49, 49);
    setFrame(movingObjectStateType::WALK, 26, 33);
    setFrame(movingObjectStateType::RUN, 50, 57);
    setFrame(movingObjectStateType::JUMP, 59, 59);
    setFrame(movingObjectStateType::DEAD, 49, 49);
    this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Koopa2"]);
    this->sprite.texture = UI::textureMap["Koopa2"];
    this->changeState(new MovingObjectWalkState(this));
}

Koopa::Koopa(bool isFlying)
    : MovingObject()
{
    setFrame(movingObjectStateType::IDLE, 49, 49);
    setFrame(movingObjectStateType::WALK, 26, 33);
    setFrame(movingObjectStateType::RUN, 50, 57);
    setFrame(movingObjectStateType::JUMP, 59, 59);
    setFrame(movingObjectStateType::DEAD, 49, 49);
    this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Koopa2"]);
    this->sprite.texture = UI::textureMap["Koopa2"];
    this->changeState(new MovingObjectWalkState(this));

    if (isFlying)
        this->changeState(new MovingObjectJumpState(this));
    else
        this->changeState(new MovingObjectWalkState(this));
}

void Koopa::update(const Vector2 &marioPos)
{
    if (!isActivated)
    {
        Vector2 pos = this->getPosition();
        float distance = Vector2Distance(marioPos, pos);
        if (distance <= activationDistance)
        {
            isActivated = true;
        }
    }
    if (isActivated)
    {
        MovingObject::update(marioPos);
    }
}

void Koopa::display()
{
    if (isActivated)
    {
        MovingObject::display();
    }
}


void Koopa::updateCollision(GameObject *other, int type)
{
    Character::updateCollision(other, type);
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

        if (type == TOP)
        {
            mario->jumpFromEnemy();
            // Check if it's a Flying Koopa (which starts in JumpState)
            if (dynamic_cast<MovingObjectJumpState *>(this->currentState))
            {
                // First hit: loses wings and becomes a walking Koopa
                SoundController::getInstance().playPlayerStateSFX(playerStateType::KICK_SHELL);
                this->changeState(new MovingObjectWalkState(this));
                GameInfo::getInstance()->addScore(50); 
                return;
            }
            // Second hit: becomes a shell
            else if (dynamic_cast<MovingObjectWalkState *>(this->currentState))
            {
                SoundController::getInstance().playPlayerStateSFX(playerStateType::KICK_SHELL);
                this->toNewBody();
                this->changeState(new MovingObjectIdleState(this));
                GameInfo::getInstance()->addScore(50); 
                return;
            }
            // Third hit: kills the shell if it's running
            else if (dynamic_cast<MovingObjectRunState *>(this->currentState))
            {
                this->changeState(new MovingObjectDeadState(this));
                return;
            }
        }

        // If Player hits an idle shell, it starts moving
        if (dynamic_cast<MovingObjectIdleState *>(this->currentState))
        {
            SoundController::getInstance().playPlayerStateSFX(playerStateType::KICK_SHELL);
            this->direction = (mario->body->GetPosition().x < this->body->GetPosition().x) ? LEFT : RIGHT;
            this->changeState(new MovingObjectRunState(this));
            return;
        }

        if (!dynamic_cast<MovingObjectIdleState *>(this->currentState) && !dynamic_cast<MovingObjectDeadState *>(this->currentState))
        {
            // change mario state accordingly when hit by enemy
            mario->hitByEnemy();
        }
    }

    Block *block = dynamic_cast<Block *>(other);
    MovingObject* enemy = dynamic_cast<MovingObject *>(other);
    // If a running shell hits another enemy, the other enemy is defeated
    if (dynamic_cast<MovingObjectRunState *>(this->currentState) && enemy && !dynamic_cast<MovingObjectDeadState *>(enemy->currentState))
    {
        if(dynamic_cast<Mushroom*>(enemy) || dynamic_cast<Star*> (enemy))
            return; // Don't defeat mushrooms
        enemy->changeState(new MovingObjectDeadState(enemy));
    }
    else if (block || enemy)
    {
        // Standard wall collision logic
        if (type == LEFTSIDE)
        {
            this->direction = LEFT;
        }
        else if (type == RIGHTSIDE)
        {
            this->direction = RIGHT;
        }
    }
}
