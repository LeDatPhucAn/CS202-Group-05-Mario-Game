#include "Lakitu.hpp"
#include "Spiny.hpp"
#include "MovingObjectState.hpp"
#include "Mario.hpp"
#include "Game.hpp"
#include "UI.hpp"
#include "raylib.h"

// LakituThrowState Implementation
LakituThrowState::LakituThrowState(Lakitu* character)
    : MovingObjectThrowState(character, 5), lakitu(character)
{
}

void LakituThrowState::executeThrow()
{
    if (lakitu)
    {
        Spiny *newSpiny = new Spiny();
        newSpiny->setPosition(lakitu->getPosition());
        newSpiny->changeState(new MovingObjectWalkState(newSpiny));
        Game::addGameObject(newSpiny);
    }
}

Lakitu::Lakitu(): MovingObject()
{
    setFrame(movingObjectStateType::IDLE, 0, 0);
    setFrame(movingObjectStateType::FLY, 0, 0);
    setFrame(movingObjectStateType::THROW, 1, 5);
    setFrame(movingObjectStateType::DEAD, 6, 6);
    this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Lakitu"]);
    this->sprite.texture = UI::textureMap["Lakitu"];

    this->throwTimer = 0.0f;
    this->changeState(new MovingObjectFlyState(this));
}

void Lakitu::update(const Vector2 &marioPos)
{
    Character::update();
    if (dynamic_cast<MovingObjectDeadState *>(this->currentState))
        return;

    // --- Movement Logic ---
    b2Vec2 vel = this->body->GetLinearVelocity();
    b2Vec2 currentPos = body->GetPosition();

    // Target position
    float targetX = (marioPos.x + 60.0f) / PPM;
    float targetY = 15.0f / PPM; // Fixed Y position

    // Horizontal movement
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

    // Vertical movement - keep at target Y
    float dy = targetY - currentPos.y;
    vel.y = dy * 10.0f; // Adjust multiplier for faster/slower Y correction

    body->SetLinearVelocity(vel);
    

    // --- Throwing Logic ---
    if (!dynamic_cast<MovingObjectThrowState*>(this->currentState))
    {
        throwTimer += GetFrameTime();
        if (throwTimer > throwCooldown)
        {
            throwTimer = 0.0f;
            this->changeState(new LakituThrowState(this));
        }
    }
}

void Lakitu::updateCollision(GameObject *other, int type)
{
    // Mario *mario = dynamic_cast<Mario *>(other);
    // if (mario)
    // {
    //     if (dynamic_cast<DeadState *>(mario->currentState) || dynamic_cast<MovingObjectDeadState *>(this->currentState))
    //     {
    //         return;
    //     }

    //     if (type == TOP)
    //     {
    //         this->changeState(new MovingObjectDeadState(this));
    //     }
    //     else 
    //     {
    //         mario->hitByMovingObject();
    //     }
    // }
}