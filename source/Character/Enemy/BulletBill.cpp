#include "BulletBill.hpp"
#include "EnemyState.hpp"
#include "Mario.hpp"
#include "Game.hpp"
#include <cmath>

BulletBill::BulletBill()
    : Enemy()
{
    setFrame(enemyStateType::IDLE, 0, 0);
    setFrame(enemyStateType::FLY, 1, 7);
    setFrame(enemyStateType::DEAD, 18, 18);
    this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["BulletBill"]);
    this->sprite.texture = UI::textureMap["BulletBill"];

    this->isActivated = false;
    this->changeState(new EnemyIdleState(this));
}

void BulletBill::update(const Vector2 &marioPos)
{
    b2Vec2 vel = this->body->GetLinearVelocity();
    vel.x = this->direction * fabs(100.f / PPM);
    vel.y = -0.75f;
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
            this->changeState(new EnemyFlyState(this));
            this->isActivated = true;
        }
    }

    Enemy::update(marioPos);
}

void BulletBill::updateCollision(GameObject *other, int type)
{
    Character::updateCollision(other, type);

    // Handle collision with Mario
    Mario *mario = dynamic_cast<Mario *>(other);
    if (mario)
    {
        if (dynamic_cast<DeadState *>(mario->currentState) || dynamic_cast<EnemyDeadState *>(this->currentState))
        {
            return;
        }

        // BulletBill can only be defeated by stomping from above
        if (type == TOP)
        {
            this->changeState(new EnemyDeadState(this));
            return;
        }
        else {
            mario->hitByEnemy();
        }
    }

    // BulletBill destroys other enemies on contact (only when activated)
    if (isActivated)
    {
        Enemy *otherEnemy = dynamic_cast<Enemy *>(other);
        if (otherEnemy && otherEnemy != this)
        {
            if (!dynamic_cast<EnemyDeadState *>(otherEnemy->currentState))
            {
                otherEnemy->changeState(new EnemyDeadState(otherEnemy));
            }
        }
    }
}