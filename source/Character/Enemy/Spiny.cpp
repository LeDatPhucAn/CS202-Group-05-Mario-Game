#include "Spiny.hpp"
#include "EnemyState.hpp"
#include "Mario.hpp"

Spiny::Spiny()
    : Enemy()
{
    setFrame(enemyStateType::IDLE, 0, 0);
    setFrame(enemyStateType::WALK, 0, 3);
    setFrame(enemyStateType::DEAD, 4, 4);
    setFrame(enemyStateType::FALL, 4, 4);
    this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Spiny"]);
    this->sprite.texture = UI::textureMap["Spiny"];
}

void Spiny::updateCollision(GameObject *other, int type)
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

    // --- Mario Collision Logic (Different from Goomba) ---
    Mario *mario = dynamic_cast<Mario *>(other);
    if (mario)
    {
        if (dynamic_cast<DeadState *>(mario->currentState) || dynamic_cast<EnemyDeadState *>(this->currentState))
        {
            return;
        }

        // A Spiny hurts Mario even if he stomps on it.
        mario->hitByEnemy();
    }
}