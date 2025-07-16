#include "Spiny.hpp"
#include "EnemyState.hpp"
#include "Mario.hpp"

Spiny::Spiny()
    : Enemy()
{
    this->sprite.StartEndFrames[IDLE] = {132, 132}; 
    this->sprite.StartEndFrames[WALK] = {132, 133}; 
    this->sprite.StartEndFrames[DEAD] = {132, 132}; 
    this->sprite.StartEndFrames[FALL] = {130, 131};
    this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Enemies2D"]);
    this->sprite.texture = UI::textureMap["Enemies2D"];
}

void Spiny::updateCollision(GameObject *other, int type)
{
    Character::updateCollision(other, type);

    Block *block = dynamic_cast<Block *>(other);
    if (block)
    {
        if (type == LEFTSIDE){
            this->direction = RIGHT;
            this->movement.velocity.x = -20.f;
        }
        else if (type == RIGHTSIDE){
            this->direction = LEFT;
            this->movement.velocity.x = 20.f;
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
        mario->changeState(new DeadState(mario));
    }
}