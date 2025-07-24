#include "EnemyState.hpp"
#include "Enemy.hpp"
#include "Character.hpp"
#include "Game.hpp"
EnemyState::EnemyState(stateType type, Character *_character, int _delay)
    : State(type, _character, _delay), character(_character) {}

EnemyIdleState::EnemyIdleState(Character *_character, int _delay)
    : EnemyState(IDLE, _character, _delay)
{
}

void EnemyIdleState::handleInput()
{
    b2Vec2 vel = character->body->GetLinearVelocity();
    vel.x = 0.0f; // Ensure no horizontal movement
    character->body->SetLinearVelocity(vel);
}

EnemyWalkState::EnemyWalkState(Character *_character, int _delay)
    : EnemyState(WALK, _character, _delay)
{
    character->direction = LEFT;
}

void EnemyWalkState::handleInput()
{
    b2Vec2 vel = character->body->GetLinearVelocity();
    vel.x = character->direction * fabs(20 / PPM); // Ensure correct direction
    character->body->SetLinearVelocity(vel);
}

EnemyRunState::EnemyRunState(Character *_character, int _delay)
    : EnemyState(RUN, _character, _delay)
{
}

void EnemyRunState::handleInput()
{
    b2Vec2 vel = character->body->GetLinearVelocity();
    vel.x = character->direction * fabs(100 / PPM); // Ensure correct direction
    character->body->SetLinearVelocity(vel);
}

EnemyDeadState::EnemyDeadState(Character *_character, int _delay)
    : EnemyState(DEAD, _character, _delay)
{
}

void EnemyDeadState::updateState()
{
    State::updateState();
    delayCounter -= GetFrameTime();
    if (delayCounter <= 0)
    {
        Enemy *enemy = dynamic_cast<Enemy *>(character);
        if (enemy)
        {
            // delete body of enemy
            enemy->beCleared = true;
            cout << "REMOVED\n";
            Game::removeEnemy(enemy);
        }
    }
}

void EnemyDeadState::handleInput()
{
    b2Vec2 vel = character->body->GetLinearVelocity();
    vel.x = 0.0f; // Ensure no horizontal movement
    character->body->SetLinearVelocity(vel);
}

EnemyFlyState::EnemyFlyState(Character *_character, int _delay)
    : EnemyState(FLY, _character, _delay) 
{
    character->direction = LEFT;
    directionTimer = 0.0f;
}

void EnemyFlyState::handleInput()
{   
    b2Vec2 vel = character->body->GetLinearVelocity();
    vel.x = character->direction * fabs(20 / PPM);
    vel.y = (sin(flyTime * flyFrequency) - 0.3f) * flyAmplitude / PPM;
    
    character->body->SetLinearVelocity(vel);

    character->body->ApplyForceToCenter({0, character->body->GetMass() * 9.8f}, true);
}

void EnemyFlyState::updateState()
{
    State::updateState();
    flyTime += GetFrameTime();
    directionTimer += GetFrameTime();

    if (directionTimer >= 3.0f)
    {
        character->direction = (character->direction == LEFT) ? RIGHT : LEFT;
        directionTimer = 0.0f; 
    }
}

