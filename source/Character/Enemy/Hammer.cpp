#include "Hammer.hpp"
#include "EnemyState.hpp"
#include "Block.hpp"
#include "Mario.hpp"

Hammer::Hammer() : Enemy()
{
    // Spin while RUN, inert when STOP (same layout as FireBall)
    setFrame(enemyStateType::RUN, 11, 11);
    setFrame(enemyStateType::STOP, 11, 11);
    setTexture("HammerBro"); // change to "Hammer" if you have a dedicated atlas
    changeState(new EnemyRunState(this));
}

void Hammer::update(const Vector2 &marioPos)
{
    // Projectile-like forward motion; gravity handled by Box2D
    b2Vec2 vel = this->body->GetLinearVelocity();
    vel.x = this->direction * fabsf(90.f / PPM); // tweak speed as needed
    this->body->SetLinearVelocity(vel);

    Enemy::update(marioPos);
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
                changeState(new EnemyStopState(this));
                break;
            default:
                break;
            }
        }
    }

    if (auto *enemy = dynamic_cast<Enemy *>(other))
    {
        if (enemy != this && !dynamic_cast<EnemyDeadState *>(enemy->currentState))
        {
            enemy->changeState(new EnemyDeadState(enemy));
        }
        changeState(new EnemyStopState(this));
    }

    if (auto *mario = dynamic_cast<Mario *>(other))
    {
        if (type == TOP)
        {
            changeState(new EnemyStopState(this));
            mario->jumpFromEnemy();
        }
    }
}