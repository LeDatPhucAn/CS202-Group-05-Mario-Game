
#include "FireBall.hpp"
FireBall::FireBall()
{
    setFrame(enemyStateType::FB_MOVE, 0, 3);
    setFrame(enemyStateType::STOP, 4, 7);
    setTexture("Projectiles2D");

    changeState(new FireBallMoveState(this));
}

void FireBall::updateCollision(GameObject *other, int type)
{
    Block *block = dynamic_cast<Block *>(other);
    if (block)
    {
        if (block->isSolid)
        {
            switch (type)
            {
            case TOP:
                // Handle top collision with solid block
                break;
            case BOTTOM:
                // Handle bottom collision with solid
                break;
            case LEFTSIDE:
            { // Handle left side collision with solid block
                changeState(new EnemyStopState(this));
                break;
            }
            case RIGHTSIDE:
                changeState(new EnemyStopState(this));
                break;
            }
        }
    }
    Enemy *enemy = dynamic_cast<Enemy *>(other);
    if (enemy)
    {
        enemy->changeState(new EnemyDeadState(enemy));
        changeState(new EnemyStopState(this));
    }
}
