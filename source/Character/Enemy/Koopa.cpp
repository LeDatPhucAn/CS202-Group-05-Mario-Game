#include "Koopa.hpp"
#include "EnemyState.hpp"
#include "Mario.hpp"
#include "Game.hpp"
#include "SoundController.hpp"
#include "Mushroom.hpp"
Koopa::Koopa()
    : Enemy()
{
    setFrame(enemyStateType::IDLE, 49, 49);
    setFrame(enemyStateType::WALK, 26, 33);
    setFrame(enemyStateType::RUN, 50, 57);
    setFrame(enemyStateType::JUMP, 59, 59);
    setFrame(enemyStateType::DEAD, 49, 49);
    this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Koopa2"]);
    this->sprite.texture = UI::textureMap["Koopa2"];
    this->changeState(new EnemyWalkState(this));
}

Koopa::Koopa(bool isFlying)
    : Enemy()
{
    setFrame(enemyStateType::IDLE, 49, 49);
    setFrame(enemyStateType::WALK, 26, 33);
    setFrame(enemyStateType::RUN, 50, 57);
    setFrame(enemyStateType::JUMP, 59, 59);
    setFrame(enemyStateType::DEAD, 49, 49);
    this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Koopa2"]);
    this->sprite.texture = UI::textureMap["Koopa2"];
    this->changeState(new EnemyWalkState(this));

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
            if (dynamic_cast<EnemyJumpState *>(this->currentState))
            {
                // First hit: loses wings and becomes a walking Koopa
                SoundController::getInstance().playMarioStateSFX(marioStateType::KICK_SHELL);
                this->changeState(new EnemyWalkState(this));
                return;
            }
            // Second hit: becomes a shell
            else if (dynamic_cast<EnemyWalkState *>(this->currentState))
            {
                SoundController::getInstance().playMarioStateSFX(marioStateType::KICK_SHELL);
                this->toNewBody();
                this->changeState(new EnemyIdleState(this));
                return;
            }
            // Third hit: kills the shell if it's running
            else if (dynamic_cast<EnemyRunState *>(this->currentState))
            {
                this->changeState(new EnemyDeadState(this));
                return;
            }
        }

        // If Mario hits an idle shell, it starts moving
        if (dynamic_cast<EnemyIdleState *>(this->currentState))
        {
            SoundController::getInstance().playMarioStateSFX(marioStateType::KICK_SHELL);
            this->direction = (mario->body->GetPosition().x < this->body->GetPosition().x) ? LEFT : RIGHT;
            this->changeState(new EnemyRunState(this));
            return;
        }

        if (!dynamic_cast<EnemyIdleState *>(this->currentState) && !dynamic_cast<EnemyDeadState *>(this->currentState))
        {
            // change mario state accordingly when hit by enemy
            mario->hitByEnemy();
        }
    }

    Block *block = dynamic_cast<Block *>(other);
    Enemy *enemy = dynamic_cast<Enemy *>(other);
    // If a running shell hits another enemy, the other enemy is defeated
    if (dynamic_cast<EnemyRunState *>(this->currentState) && enemy && !dynamic_cast<EnemyDeadState *>(enemy->currentState))
    {
        if(dynamic_cast<Mushroom *>(enemy))
            return; // Don't defeat mushrooms
        enemy->changeState(new EnemyDeadState(enemy));
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
