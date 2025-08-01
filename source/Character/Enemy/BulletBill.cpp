#include "BulletBill.hpp"
#include "EnemyState.hpp"
#include "Mario.hpp"
#include "Game.hpp"
#include <cmath>

BulletBill::BulletBill()
    : Enemy()
{
    setFrame(enemyStateType::IDLE, 19, 19);
    setFrame(enemyStateType::FLY, 20, 20); 
    setFrame(enemyStateType::DEAD, 19, 19); 
    this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Enemies2D"]);
    this->sprite.texture = UI::textureMap["Enemies2D"];
    
    
    this->isActivated = false;
    this->changeState(new EnemyIdleState(this));
}

void BulletBill::update(const Vector2& marioPos)
{
    b2Vec2 vel = this->body->GetLinearVelocity();
    vel.x = this->direction * fabs(100.f / PPM);
    vel.y = -0.55f;
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