
#include "Star.hpp"
#include "EnemyState.hpp"
#include "Mario.hpp"
#include "Score.hpp"

Star::Star()
{
    setFrame(enemyStateType::RUN, 1,1);
    setTexture("Items");

    changeState(new EnemyWalkState(this));
}

void Star::updateCollision(GameObject *other, int type)
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
        mario->EatStar();
        changeState(new EnemyStopState(this));
    }
}