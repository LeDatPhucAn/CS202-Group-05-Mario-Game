#include "Mario.hpp"
#include "Enemy.hpp" // Needed for dynamic_cast to Enemy
#include "EnemyState.hpp"
#include "Structs.hpp"
#include "FireBall.hpp"
#include "ProjectileState.hpp"
#include "Game.hpp"
Mario::Mario()
    : Character()
{
    setFrame(marioStateType::IDLE, 0, 0);
    setFrame(marioStateType::WALK, 1, 3);
    setFrame(marioStateType::RUN, 1, 3);
    setFrame(marioStateType::JUMP, 5, 5);
    setFrame(marioStateType::FALL, 5, 5);
    setFrame(marioStateType::SKID, 4, 4);
    setFrame(marioStateType::CROUCH, 0, 0);
    setFrame(marioStateType::GROW, 44, 50);
    setFrame(marioStateType::UNGROW, 50, 44);
    setFrame(marioStateType::DEAD, 6, 6);

    setTexture("Mario2D");
}
void Mario::changeForm(MarioForm form)
{
    switch (form)
    {
    case SMALL:
        setFrame(marioStateType::IDLE, 0, 0);
        setFrame(marioStateType::JUMP, 5, 5);
        setFrame(marioStateType::FALL, 5, 5);
        setFrame(marioStateType::SKID, 4, 4);
        setFrame(marioStateType::WALK, 1, 3);
        setFrame(marioStateType::RUN, 1, 3);
        setFrame(marioStateType::CROUCH, 0, 0);
        setFrame(marioStateType::GROW, 44, 50);
        setFrame(marioStateType::UNGROW, 0, 0);
        break;

    case BIG:
        setFrame(marioStateType::IDLE, 13, 13);
        setFrame(marioStateType::WALK, 16, 14);
        setFrame(marioStateType::JUMP, 18, 18);
        setFrame(marioStateType::FALL, 18, 18);
        setFrame(marioStateType::SKID, 17, 17);
        setFrame(marioStateType::RUN, 16, 14);
        setFrame(marioStateType::CROUCH, 19, 19);
        setFrame(marioStateType::GROW, 28, 28);
        setFrame(marioStateType::UNGROW, 50, 44);
        break;

    case FIRE:
        setFrame(marioStateType::IDLE, 28, 28);
        setFrame(marioStateType::WALK, 31, 29);
        setFrame(marioStateType::JUMP, 34, 34);
        setFrame(marioStateType::FALL, 34, 34);
        setFrame(marioStateType::SKID, 33, 33);
        setFrame(marioStateType::RUN, 31, 29);
        setFrame(marioStateType::CROUCH, 35, 35);
        setFrame(marioStateType::GROW, 28, 28);
        setFrame(marioStateType::UNGROW, 13, 13);
        break;

    default:
        break;
    }
}
void Mario::update()
{
    Character::update();
    sinceLastThrow += GetFrameTime();
    if (form == FIRE && IsKeyPressed(KEY_Z))
    {
        if (sinceLastThrow > throwPerSecond)
        {
            throwFireBall();
        }
    }
}
void Mario::throwFireBall()
{
    FireBall *projectile = new FireBall();
    projectile->setDirection(this->getDirection());
    Vector2 pos = this->getPositionAdapter().toPixels();
    pos.x += this->getDirection() * getSize().x;
    projectile->setPosition(pos);

    projectile->changeState(new ProjectileMoveState(projectile));
    cout << "FireBall before: " << projectile << "\n";
    Game::addGameObject(projectile);
}

void Mario::updateCollision(GameObject *other, int type)
{
    if (dynamic_cast<DeadState *>(this->currentState))
    {
        return;
    }
    Character::updateCollision(other, type);
    Enemy *enemy = dynamic_cast<Enemy *>(other);
    if (enemy)
    {
        if (dynamic_cast<DeadState *>(this->currentState) || dynamic_cast<EnemyDeadState *>(enemy->currentState))
        {
            return;
        }
        if (type == BOTTOM)
        {
            float mass = this->body->GetMass();
            b2Vec2 impulse(0, mass * jumpVel / 1.5f);
            this->body->ApplyLinearImpulseToCenter(impulse, true);
        }
        else
        {
            if (dynamic_cast<EnemyIdleState *>(enemy->currentState))
            {
                return;
            }
            this->changeState(new DeadState(this));
        }
    }
}
