
#include "Star.hpp"
#include "MovingObjectState.hpp"
#include "Player.hpp"
#include "Score.hpp"

Star::Star()
{
    setFrame(movingObjectStateType::STAR_MOVE, 3, 3);
    setTexture("Items");

    changeState(new StarMoveState(this));
}

void Star::updateCollision(GameObject *other, int type)
{
    Character::updateCollision(other, type);
    Block *block = dynamic_cast<Block *>(other);
    MovingObject *enemy = dynamic_cast<MovingObject *>(other);
    if (block || enemy)
    {
        if (type == LEFTSIDE)
        {
            this->direction = LEFT;
        }
        else if (type == RIGHTSIDE)
        {
            this->direction = RIGHT;
        }
    }

    Player *mario = dynamic_cast<Player *>(other);
    if (mario)
    {
        GameInfo::getInstance()->addScore(100);
        mario->EatStar();
        changeState(new MovingObjectStopState(this));
    }
}