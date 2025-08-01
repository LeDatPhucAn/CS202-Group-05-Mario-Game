#include "EnemyState.hpp"
#include "Enemy.hpp"
#include "Character.hpp"
#include "Game.hpp"
EnemyState::EnemyState(int type, Character *_character, int _delay)
    : State(type, _character, _delay), character(_character) {}

EnemyIdleState::EnemyIdleState(Character *_character, int _delay)
    : EnemyState((int)enemyStateType::IDLE, _character, _delay)
{
}

void EnemyIdleState::handleInput()
{
    b2Vec2 vel = character->body->GetLinearVelocity();
    vel.x = 0.0f; // Ensure no horizontal movement
    character->body->SetLinearVelocity(vel);
}

EnemyWalkState::EnemyWalkState(Character *_character, int _delay)
    : EnemyState((int)enemyStateType::WALK, _character, _delay)
{
    character->direction = RIGHT;
}

void EnemyWalkState::handleInput()
{
    b2Vec2 vel = character->body->GetLinearVelocity();
    vel.x = character->direction * fabs(20 / PPM); // Ensure correct direction
    character->body->SetLinearVelocity(vel);
}

EnemyRunState::EnemyRunState(Character *_character, int _delay)
    : EnemyState((int)enemyStateType::RUN, _character, _delay)
{
}

void EnemyRunState::handleInput()
{
    b2Vec2 vel = character->body->GetLinearVelocity();
    vel.x = character->direction * fabs(100 / PPM); // Ensure correct direction
    character->body->SetLinearVelocity(vel);
}

EnemyDeadState::EnemyDeadState(Character *_character, int _delay)
    : EnemyState((int)enemyStateType::DEAD, _character, _delay)
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
            enemy->needDeletion = true;
            // Game::removeGameObject(enemy);
        }
    }
}

void EnemyDeadState::handleInput()
{
    b2Vec2 vel = character->body->GetLinearVelocity();
    vel.x = 0.0f; // Ensure no horizontal movement
    character->body->SetLinearVelocity(vel);
}
