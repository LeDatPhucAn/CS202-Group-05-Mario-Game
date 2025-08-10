#include "HammerBro.hpp"
#include "Hammer.hpp"
#include "Game.hpp"
#include "MovingObjectState.hpp"
#include "Mario.hpp"
#include "Block.hpp"
#include <raylib.h>

HammerBro::HammerBro() : MovingObject()
{
    // Reuse walk animation while idle/jumping; no horizontal motion
    setFrame(movingObjectStateType::IDLE, 12, 24);
    setFrame(movingObjectStateType::DEAD, 10, 10);
    setTexture("HammerBro"); // change to "HammerBro" if you have a dedicated atlas
    changeState(new MovingObjectIdleState(this));
}

void HammerBro::update(const Vector2 &marioPos)
{

    float now = (float)GetTime();
    // if (isOnGround && (now - lastJumpTime) >= jumpInterval)
    // {
    //     float impulseY = -this->body->GetMass() * jumpImpulse; // upward
    //     this->body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, impulseY), true);
    //     isOnGround = false;
    //     lastJumpTime = now;
    // }

    // Throw hammer
    if ((now - lastThrowTime) >= throwInterval)
    {
        auto* h = new Hammer();
        h->setPosition({ this->getPosition().x, this->getPosition().y - 8 }); // small offset
        h->setDirection(this->direction); // if you have setter; otherwise ensure default
        Game::addGameObject(h);
        lastThrowTime = now;
    }

    MovingObject::update(marioPos);
}

void HammerBro::updateCollision(GameObject *other, int type)
{
    Character::updateCollision(other, type);

    if (auto *block = dynamic_cast<Block *>(other))
    {
        if (block->isSolid && type == TOP)
        {
            isOnGround = true;
        }
    }

    if (auto *mario = dynamic_cast<Mario *>(other))
    {
        if (dynamic_cast<DeadState *>(mario->currentState) ||
            dynamic_cast<MovingObjectDeadState *>(this->currentState))
            return;

        if (type == TOP)
        {
            this->changeState(new MovingObjectDeadState(this));
            mario->jumpFromEnemy();
        }
        else
        {
            mario->hitByEnemy();
        }
    }
}