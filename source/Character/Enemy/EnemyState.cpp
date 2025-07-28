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
    vel.x = 0.0f;
    character->body->SetLinearVelocity(vel);
}

EnemyWalkState::EnemyWalkState(Character *_character, int _delay)
    : EnemyState((int)enemyStateType::WALK, _character, _delay)
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
    : EnemyState((int)enemyStateType::RUN, _character, _delay)
{
}

void EnemyRunState::handleInput()
{
    b2Vec2 vel = character->body->GetLinearVelocity();
    vel.x = character->direction * fabs(200 / PPM); // Ensure correct direction
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

// --- FLY STATE IMPLEMENTATION ---

EnemyFlyState::EnemyFlyState(Character* _character)
    : EnemyState((int)enemyStateType::FLY, _character, 5)
{
    // On entering this state, ensure gravity is disabled.
    if (character && character->body)
    {
        character->body->SetGravityScale(0.0f);
    }
}

EnemyFlyState::~EnemyFlyState()
{
    // Optional: Restore gravity if the character can stop flying.
    if (character && character->body)
    {
        character->body->SetGravityScale(1.0f);
    }
}

void EnemyFlyState::handleInput()
{
    
}


// --- JUMP STATE IMPLEMENTATION ---

EnemyJumpState::EnemyJumpState(Character* _character, int delay)
  : EnemyState((int)enemyStateType::JUMP, _character, delay)
{
    if (character && character->body) 
        character->body->SetGravityScale(0.0f);
    initSineJump();
}

EnemyJumpState::~EnemyJumpState()
{
    // restore normal gravity when (if) we ever exit this state
    if (character && character->body)
        character->body->SetGravityScale(1.0f);
}

void EnemyJumpState::initSineJump()
{
    elapsed   = 0.0f;
    if (character && character->body) 
        baselineY = character->body->GetPosition().y;
}

void EnemyJumpState::handleInput()
{
    if (!character || !character->body) return;

    // maintain horizontal patrol speed
    b2Vec2 v = character->body->GetLinearVelocity();
    v.x = character->direction * horizontalSpeed;
    character->body->SetLinearVelocity(v);
}

void EnemyJumpState::updateState()
{
    State::updateState();

    // advance our time
    float dt = GetFrameTime();
    elapsed += dt;

    // compute vertical velocity from derivative of y = A * sin(ω t)
    float vy = amplitude * omega * cos(omega * elapsed);

    // set body velocity (Box2D works in meters/sec)
    b2Vec2 v = character->body->GetLinearVelocity();
    v.y = vy;
    character->body->SetLinearVelocity(v);

    // (optional) wrap elapsed to avoid large floats
    if (elapsed > period) elapsed -= period;
}
