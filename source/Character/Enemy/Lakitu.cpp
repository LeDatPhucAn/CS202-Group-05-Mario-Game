#include "Lakitu.hpp"
#include "Spiny.hpp" // Lakitu needs to know about Spiny to throw it
#include "EnemyState.hpp"
#include "Mario.hpp"
#include "Game.hpp"


Lakitu::Lakitu()
    : Enemy()
{

    this->sprite.StartEndFrames[IDLE] = {15, 15}; // Normal flying
    this->sprite.StartEndFrames[WALK] = {15, 15}; // Using WALK state for flying animation
    this->sprite.StartEndFrames[DEAD] = {16, 16}; 
    this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Enemies2D"]);
    this->sprite.texture = UI::textureMap["Enemies2D"];

    // Lakitu flies, so it is not affected by gravity
    this->movement.acceleration.y = 0;
    this->throwTimer = 0.0f;
}

void Lakitu::setTarget(Mario* marioTarget, Game* game)
{
    this->target = marioTarget;
    this->game = game; 
}

void Lakitu::update()
{
    // Call base update for animations
    Character::update();

    if (dynamic_cast<DeadState*>(target->currentState) || dynamic_cast<EnemyDeadState*>(this->currentState)) return; 

    // --- Movement Logic ---
    // Lakitu tries to hover above and slightly ahead of Mario
    Vector2 targetPos = { target->getPosition().x + 32, this->pos.y };

    this->pos.x += (targetPos.x - this->pos.x) * 0.02f;
    this->pos.y= 0.f;
    // --- Throwing Logic ---
    throwTimer += GetFrameTime();
    if (throwTimer > throwCooldown)
    {
        throwTimer = 0.0f;

        // Create a new Spiny at Lakitu's position
        Spiny* newSpiny = new Spiny();
        newSpiny->setPosition(this->pos);
        newSpiny->changeState(new EnemyWalkState(newSpiny));
        game->addEnemy(newSpiny);
    }
}

void Lakitu::updateCollision(GameObject *other, int type)
{
    Mario *mario = dynamic_cast<Mario *>(other);
    if (mario)
    {
        if (dynamic_cast<DeadState *>(mario->currentState) || dynamic_cast<EnemyDeadState *>(this->currentState))
        {
            return;
        }

        if (type == HEAD)
        {
            this->changeState(new EnemyDeadState(this));
        }
    }
}