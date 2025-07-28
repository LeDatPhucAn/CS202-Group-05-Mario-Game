#include "HammerBro.hpp"
#include "EnemyState.hpp"
#include "Mario.hpp"
#include "Block.hpp"
#include "Game.hpp"

Vector2 getBodySize(b2Body* body)
{
    b2Fixture* fixture = body->GetFixtureList();
    if (!fixture) return {0, 0};
    b2PolygonShape* poly = dynamic_cast<b2PolygonShape*>(fixture->GetShape());
    if (!poly) return {0, 0};
    float minX = poly->m_vertices[0].x, maxX = poly->m_vertices[0].x;
    float minY = poly->m_vertices[0].y, maxY = poly->m_vertices[0].y;
    for (int i = 1; i < poly->m_count; i++)
    {
        if (poly->m_vertices[i].x < minX) minX = poly->m_vertices[i].x;
        if (poly->m_vertices[i].x > maxX) maxX = poly->m_vertices[i].x;
        if (poly->m_vertices[i].y < minY) minY = poly->m_vertices[i].y;
        if (poly->m_vertices[i].y > maxY) maxY = poly->m_vertices[i].y;
    }
    return {maxX - minX, maxY - minY};
}

Hammer::Hammer()
    : Enemy()
{
    // Assuming sprite index 24 is the hammer sprite in the "Enemies2D" atlas
    setFrame(enemyStateType::IDLE, 25, 25);
    setFrame(enemyStateType::FLY, 25, 25);
    setFrame(enemyStateType::DEAD, 25, 25);
    this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Enemies2D"]);
    this->sprite.texture = UI::textureMap["Enemies2D"];
    
}



void Hammer::update(const Vector2& marioPos)
{
    // A hammer is a simple projectile, its state update is just checking its age.
    Character::update();

    age += GetFrameTime();
    if (age >= lifetime)
    {
        this->beCleared = true;
    }

    if (this->body)
    {

        const float rotationSpeed = 40.0f; 
        this->body->SetAngularVelocity(rotationSpeed);
    }
}

void Hammer::updateCollision(GameObject *other, int type)
{
    // Hammers are destroyed on contact with Mario or solid blocks.
    if (dynamic_cast<Mario *>(other) || dynamic_cast<Block *>(other))
    {
        this->beCleared = true;
    }
}

HammerBro::HammerBro()
    : Enemy()
{
    setFrame(enemyStateType::IDLE, 21, 22);   // Hopping animation
    setFrame(enemyStateType::JUMP, 23, 23);   // Frame for throwing
    setFrame(enemyStateType::DEAD, 22, 22);   // Use one of the frames, will be flipped
    this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Enemies2D"]);
    this->sprite.texture = UI::textureMap["Enemies2D"];

    // Initialize timers with a random offset to de-synchronize multiple Hammer Bros
    this->jumpTimer = (float)(GetRandomValue(0, 150)) / 100.0f;
    this->throwTimer = (float)(GetRandomValue(0, 100)) / 100.0f;
}

void HammerBro::update(const Vector2& marioPos)
{
    Character::update();

    if (dynamic_cast<EnemyDeadState *>(this->currentState))
    {
        return; 
    }

    // Always face Mario
    this->direction = (marioPos.x < this->getPosition().x) ? LEFT : RIGHT;

    // // --- Jumping Logic ---
    // jumpTimer += GetFrameTime();
    // // To prevent jumping in mid-air, we check if it's in a state that allows jumping (e.g., not already jumping)
    // if (jumpTimer >= jumpCooldown && dynamic_cast<EnemyWalkState*>(this->currentState))
    // {
    //     jumpTimer = 0.0f;
    //     // Use the new jump state for a controlled arc
    //     this->changeState(new EnemyJumpState(this));
    // }

    // --- Throwing Logic ---
    throwTimer += GetFrameTime();
    if (throwTimer >= throwCooldown)
    {
        throwTimer = 0.0f;

        Hammer* hammer = new Hammer();
        
        Vector2 startPos = this->getPosition();
        startPos.y -= getBodySize(this->body).y / 2;
        startPos.x += (this->direction == RIGHT ? getBodySize(this->body).x / 2 : -getBodySize(this->body).x / 2);
        hammer->setPosition(startPos);
        hammer->changeState(new EnemyFlyState(hammer));
        Game::addEnemy(hammer);

        // Give the hammer its initial arcing velocity
        float throwSpeedX = 4.0f * this->direction;
        float throwSpeedY = -10.5f; // A strong upward velocity for the arc
        hammer->getBody()->SetLinearVelocity({throwSpeedX, throwSpeedY});
    }
}

void HammerBro::updateCollision(GameObject *other, int type)
{
    Character::updateCollision(other, type);
    Mario *mario = dynamic_cast<Mario *>(other);

    if (mario)
    {
        if (dynamic_cast<DeadState *>(mario->currentState) || dynamic_cast<EnemyDeadState *>(this->currentState))
        {
            return;
        }

        // Hammer Bro is defeated by being stomped on
        if (type == TOP)
        {
            this->changeState(new EnemyDeadState(this));

            return;
        }
    }
    if (dynamic_cast<Enemy*>(other))
    {
        return;
    }
}