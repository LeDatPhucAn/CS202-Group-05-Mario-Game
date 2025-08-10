#include "Hammer.hpp"
#include "MovingObjectState.hpp"
#include "Block.hpp"
#include "Player.hpp"

Hammer::Hammer() : MovingObject()
{
    // Spin while RUN, inert when DEAD (same layout as FireBall)
    setFrame(movingObjectStateType::RUN, 11, 11);
    setFrame(movingObjectStateType::DEAD, 11, 11);
    setTexture("HammerBro"); // change to "Hammer" if you have a dedicated atlas
    changeState(new MovingObjectRunState(this));
}

void Hammer::update(const Vector2 &marioPos)
{
    // Projectile-like forward motion; gravity handled by Box2D
    b2Vec2 vel = this->body->GetLinearVelocity();
    vel.x = this->direction * fabsf(90.f / PPM); // tweak speed as needed
    this->body->SetLinearVelocity(vel);

    MovingObject::update(marioPos);
}

void Hammer::updateCollision(GameObject *other, int type)
{
    if (auto *block = dynamic_cast<Block *>(other))
    {
        if (block->isSolid)
        {
            switch (type)
            {
            case TOP:
            case BOTTOM:
            case LEFTSIDE:
            case RIGHTSIDE:
                changeState(new MovingObjectDeadState(this));
                break;
            default:
                break;
            }
        }
    }

    if (auto *enemy = dynamic_cast<MovingObject *>(other))
    {
        if (enemy != this && !dynamic_cast<MovingObjectDeadState *>(enemy->currentState))
        {
            enemy->changeState(new MovingObjectDeadState(enemy));
        }
        changeState(new MovingObjectDeadState(this));
    }

    if (auto *mario = dynamic_cast<Player *>(other))
    {
        if (type == TOP)
        {
            changeState(new MovingObjectDeadState(this));
            mario->jumpFromEnemy();
        }
    }
}