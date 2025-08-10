#include "Spiny.hpp"
#include "MovingObjectState.hpp"
#include "Player.hpp"

Spiny::Spiny()
    : MovingObject()
{
    setFrame(movingObjectStateType::IDLE, 0, 0);
    setFrame(movingObjectStateType::WALK, 0, 3);
    setFrame(movingObjectStateType::DEAD, 4, 4);
    setFrame(movingObjectStateType::FALL, 4, 4);
    this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Spiny"]);
    this->sprite.texture = UI::textureMap["Spiny"];
}

void Spiny::updateCollision(GameObject *other, int type)
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

    // --- Player Collision Logic (Different from Goomba) ---
    Player *mario = dynamic_cast<Player *>(other);
    if (mario)
    {
        if (dynamic_cast<DeadState *>(mario->currentState) || dynamic_cast<MovingObjectDeadState *>(this->currentState))
        {
            return;
        }

        // A Spiny hurts Player even if he stomps on it.
        mario->hitByEnemy();
    }
}