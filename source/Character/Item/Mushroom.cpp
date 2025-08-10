#include "Mushroom.hpp"
#include "MovingObjectState.hpp"
#include "Player.hpp"
#include "Score.hpp"

Mushroom::Mushroom()
{
    setFrame(movingObjectStateType::WALK, 0, 0);
    setTexture("Items");

    changeState(new MovingObjectWalkState(this));
}

void Mushroom::updateCollision(GameObject *other, int type)
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
        if (mario->getForm() != FIRE)
            mario->changeState(new GrowState(mario));
        changeState(new MovingObjectStopState(this));
    }
}