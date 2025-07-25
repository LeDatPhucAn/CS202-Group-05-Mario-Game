#include "PiranhaPlant.hpp"
#include "EnemyState.hpp"
#include "Mario.hpp"

#include "PiranhaPlant.hpp"
#include "EnemyState.hpp"
#include "Mario.hpp"

PiranhaPlant::PiranhaPlant()
    : Enemy()
{
    this->sprite.StartEndFrames[IDLE] = {9, 10};
    this->sprite.StartEndFrames[DEAD] = {9, 10};
    this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Enemies2D"]);
    this->sprite.texture = UI::textureMap["Enemies2D"];

    // // Piranha Plants do not move via physics
    // this->movement.velocity = {0, 0};
    // this->movement.acceleration = {0, 0};

    // Defer position-based setup to the first update call
    this->timer = 0.0f;
    this->isHiding = true; // Start hidden inside the pipe
    this->isSetup = false;
}

void PiranhaPlant::update(const Vector2 &marioPos)
{
    // One-time setup on the first frame to capture the correct positions
    if (!isSetup)
    {

        this->emergedPos = this->getPosition();
        this->hiddenPos = {this->getPosition().x, this->getPosition().y + 24.0f};
        this->pos = this->hiddenPos;
        isSetup = true;
    }

    Character::update();

    timer += GetFrameTime();

    if (isHiding && timer > hideDuration)
    {
        isHiding = false; // Time to emerge
        timer = 0.0f;
    }
    else if (!isHiding && timer > emergeDuration)
    {
        isHiding = true; // Time to hide
        timer = 0.0f;
    }

    Vector2 targetPos = isHiding ? hiddenPos : emergedPos;

    // Move towards the target position
    // this->pos.y += (targetPos.y - this->pos.y) * 0.1f;

    b2Vec2 vel = this->body->GetLinearVelocity();

    Vector2 currentPos = this->getPosition();
    float desiredSpeed = 2.0f;

    float dy = targetPos.y - currentPos.y;
    float epsilon = 0.1f; // To avoid jittering

    if (fabs(dy) > epsilon)
    {
        vel.y = desiredSpeed * (dy > 0 ? 1.0f : -1.0f); // sign of dy determines direction
    }
    else
    {
        vel.y = 0.0f; 
    }

    body->SetLinearVelocity(vel);
}

void PiranhaPlant::updateCollision(GameObject *other, int type)
{
    // Piranha Plants do not react to blocks or other enemies
    if (dynamic_cast<Block *>(other) || (dynamic_cast<Enemy *>(other) && other != this))
    {
        return;
    }

    Mario *mario = dynamic_cast<Mario *>(other);
    if (mario)
    {
        if (dynamic_cast<DeadState *>(mario->currentState) || dynamic_cast<EnemyDeadState *>(this->currentState))
        {
            return;
        }

        mario->changeState(new DeadState(mario));
    }
}