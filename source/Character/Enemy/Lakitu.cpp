#include "Lakitu.hpp"
#include "Spiny.hpp" // Lakitu needs to know about Spiny to throw it
#include "EnemyState.hpp"
#include "Mario.hpp"
#include "Game.hpp"

Lakitu::Lakitu()
    : Enemy()
{

    setFrame(enemyStateType::IDLE, 15, 15); // Normal flying
    setFrame(enemyStateType::FLY, 15, 15);  // Using FLY state for flying animation
    setFrame(enemyStateType::DEAD, 16, 16);
    this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Enemies2D"]);
    this->sprite.texture = UI::textureMap["Enemies2D"];

    // Lakitu flies, so it is not affected by gravity
    this->throwTimer = 0.0f;
    this->changeState(new EnemyIdleState(this));
}

void Lakitu::update(const Vector2 &marioPos)
{
    Character::update();
    if (dynamic_cast<EnemyDeadState *>(this->currentState))
        return;

    // --- Movement Logic ---
    b2Vec2 vel = this->body->GetLinearVelocity();
    vel.y = -0.75f;

    // Use the marioPos parameter for movement
    b2Vec2 currentPos = body->GetPosition();
    float targetX = (marioPos.x + 32.0f) / PPM;

    float desiredSpeed = 4.0f;
    float dx = targetX - currentPos.x;
    float epsilon = 0.1f;

    if (fabs(dx) > epsilon)
    {
        vel.x = desiredSpeed * (dx > 0 ? 1.0f : -1.0f);
    }
    else
    {
        vel.x = 0.0f;
    }

    body->SetLinearVelocity(vel);
    // Apply an UPWARD force to counteract gravity
    this->body->ApplyForceToCenter({0, this->body->GetMass() * fallGravity}, true);

    // --- Throwing Logic ---
    throwTimer += GetFrameTime();
    if (throwTimer > throwCooldown)
    {
        throwTimer = 0.0f;

        Spiny *newSpiny = new Spiny();
        newSpiny->setPosition(this->getPosition());
        newSpiny->changeState(new EnemyWalkState(newSpiny));
        Game::addGameObject(newSpiny);
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
        else
        {
            mario->hitByEnemy();
        }
    }
}