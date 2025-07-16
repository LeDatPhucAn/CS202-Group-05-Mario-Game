#include "EnemyState.hpp"
#include "Enemy.hpp"
#include "Character.hpp"

EnemyState::EnemyState(stateType type, Character *_character, int _delay)
    : State(type, _character, _delay), character(_character) {}

EnemyIdleState::EnemyIdleState(Character *_character, int _delay)
    : EnemyState(IDLE, _character, _delay)
{
    b2Vec2 vel = character->body->GetLinearVelocity();
    character->body->SetLinearVelocity({0, vel.y});
}

void EnemyIdleState::handleInput()
{
}

EnemyWalkState::EnemyWalkState(Character *_character, int _delay)
    : EnemyState(WALK, _character, _delay)
{
    // character->movement.velocity.x = -20.f;
    b2Vec2 vel = character->body->GetLinearVelocity();
    character->body->SetLinearVelocity({-200.f / PPM, vel.y});
    character->direction = RIGHT;
}

void EnemyWalkState::handleInput()
{
    b2Vec2 vel = character->body->GetLinearVelocity();
    if (vel.x == 0)
    {
        character->direction = (character->direction == LEFT) ? RIGHT : LEFT;
        vel.x = (character->direction == LEFT) ? -200.f / PPM : 200.f / PPM;
        character->body->SetLinearVelocity({vel.x, vel.y});
    }
}

EnemyRunState::EnemyRunState(Character *_character, int _delay)
    : EnemyState(RUN, _character, _delay)
{
    b2Vec2 vel = character->body->GetLinearVelocity();
    if (character->direction == RIGHT)
    {
        // character->movement.velocity.x = -200.f;
        character->body->SetLinearVelocity({-200.f / PPM, vel.y});
    }
    else
    {
        // character->movement.velocity.x = 200.f;
        character->body->SetLinearVelocity({200.f / PPM, vel.y});
    }
}

void EnemyRunState::handleInput()
{
    b2Vec2 vel = character->body->GetLinearVelocity();
    if (vel.x == 0)
    {
        character->direction = (character->direction == LEFT) ? RIGHT : LEFT;
        vel.x = (character->direction == LEFT) ? -200.f / PPM : 200.f / PPM;
        character->body->SetLinearVelocity({vel.x, vel.y});
    }
}

EnemyDeadState::EnemyDeadState(Character *_character, int _delay)
    : EnemyState(DEAD, _character, _delay)
{
    // character->movement.velocity = {0, 0};
    // character->movement.acceleration = {0, 0};
    character->body->SetLinearVelocity({0, 0});
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
            enemy->beCleared = true;
        }
    }
}

void EnemyDeadState::handleInput()
{
}