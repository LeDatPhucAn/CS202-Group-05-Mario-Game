
#include "FireBall.hpp"
FireBall::FireBall()
{
    setFrame(movingObjectStateType::FB_MOVE, 0, 3);
    setFrame(movingObjectStateType::STOP, 4, 7);
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
                changeState(new MovingObjectStopState(this));
                break;
            }
            case RIGHTSIDE:
                changeState(new MovingObjectStopState(this));
                break;
            }
        }
    }
    MovingObject *enemy = dynamic_cast<MovingObject *>(other);
    if (enemy)
    {
        enemy->changeState(new MovingObjectDeadState(enemy));
        changeState(new MovingObjectStopState(this));
    }
}
