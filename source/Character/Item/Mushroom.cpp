
#include "Mushroom.hpp"
#include "EnemyState.hpp"
#include "Mario.hpp"
#include "Score.hpp"

Mushroom::Mushroom()
{
    setFrame(enemyStateType::WALK, 0, 0);
    setTexture("Items");

    changeState(new EnemyWalkState(this));
}

void Mushroom::updateCollision(GameObject *other, int type)
{
    Character::updateCollision(other, type);
    Block *block = dynamic_cast<Block *>(other);
    Enemy *enemy = dynamic_cast<Enemy *>(other);
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

    Mario *mario = dynamic_cast<Mario *>(other);
    if (mario)
    {
        Score::getInstance()->addScore(100);
        if (mario->getForm() != FIRE)
            mario->changeState(new GrowState(mario));
        changeState(new EnemyStopState(this));
    }
}