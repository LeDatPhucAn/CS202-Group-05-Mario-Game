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
    this->throwTimer = 0.0f;
}

void Lakitu::setTarget(Mario *marioTarget, Game *game)
{
    this->target = marioTarget;
    this->game = game;
}

void Lakitu::update()
{

    // Call base update for animations
    Character::update();

    if (dynamic_cast<DeadState *>(target->currentState) || dynamic_cast<EnemyDeadState *>(this->currentState))
        return;

    // --- Movement Logic ---

    // Not Affected by gravity
    b2Vec2 vel = this->body->GetLinearVelocity();
    vel.y = 0.0f; // Reset vertical velocity to zero

    // Lakitu tries to hover above and slightly ahead of Mario
    b2Vec2 currentPos = body->GetPosition();
    float targetX = (target->pos.toPixels().x + 32.0f) / PPM; 
    float desiredSpeed = 2.0f;                              

    float dx = targetX - currentPos.x;
    float threshold = 0.1f; // To avoid jittering

    if (fabs(dx) > threshold)
    {
        vel.x = desiredSpeed * (dx > 0 ? 2.0f : -2.0f);
    }
    else
    {
        vel.x = 0.0f; // Close enough, stop horizontal movement
    }

    body->SetLinearVelocity(vel);
    this->body->ApplyForceToCenter({0, this->body->GetMass() * (-addedFallGravity)}, true);

    // --- Throwing Logic ---
    throwTimer += GetFrameTime();
    if (throwTimer > throwCooldown)
    {
        throwTimer = 0.0f;

        // Create a new Spiny at Lakitu's position
        Spiny *newSpiny = new Spiny();
        newSpiny->setPosition(this->getPosition());
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

        if (type == TOP)
        {
            this->changeState(new EnemyDeadState(this));
        }
    }
}