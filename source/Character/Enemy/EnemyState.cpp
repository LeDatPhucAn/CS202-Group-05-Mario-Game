#include "EnemyState.hpp"
#include "Enemy.hpp"
#include "Character.hpp"
#include "Game.hpp"
#include "Score.hpp"
#include "SoundController.hpp"
EnemyState::EnemyState(int type, Character *_character, int _delay)
    : State(type, _character, _delay), character(_character) {}

EnemyIdleState::EnemyIdleState(Character *_character, int _delay)
    : EnemyState((int)enemyStateType::IDLE, _character, _delay)
{
}

void EnemyIdleState::handleInput()
{
    b2Vec2 vel = character->getBody()->GetLinearVelocity();
    vel.x = 0.0f;
    character->getBody()->SetLinearVelocity(vel);
}

EnemyWalkState::EnemyWalkState(Character *_character, int _delay)
    : EnemyState((int)enemyStateType::WALK, _character, _delay)
{
    character->direction = RIGHT;
}

void EnemyWalkState::handleInput()
{
    b2Vec2 vel = character->getBody()->GetLinearVelocity();
    vel.x = -character->direction * fabs(20 / PPM); // Ensure correct direction
    character->getBody()->SetLinearVelocity(vel);
}

EnemyRunState::EnemyRunState(Character *_character, int _delay)
    : EnemyState((int)enemyStateType::RUN, _character, _delay)
{
}

void EnemyRunState::handleInput()
{
    b2Vec2 vel = character->getBody()->GetLinearVelocity();
    vel.x = -character->direction * fabs(200 / PPM); // Ensure correct direction
    character->getBody()->SetLinearVelocity(vel);
}

EnemyDeadState::EnemyDeadState(Character *_character, int _delay)
    : EnemyState((int)enemyStateType::DEAD, _character, _delay)
{
    // make a sound when an enemy dies
    SoundController::getInstance().playMarioStateSFX(marioStateType::SQUISH_ENEMY);
    Score::getInstance()->addScore(100); // Add score for defeating enemy
    character->isGrounded = false;
    character->body->SetLinearVelocity({0, -250.f / PPM});
    b2Body *body = character->getBody();
    b2Filter filter;
    filter.categoryBits = 0;
    filter.maskBits = 0;

    // Iterate over all fixtures attached to this body
    for (b2Fixture *fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
    {
        fixture->SetFilterData(filter);
    }
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
        }
    }
}
void EnemyDeadState::handleInput()
{
    b2Vec2 vel = character->getBody()->GetLinearVelocity();
    vel.x = 0.0f; // Ensure no horizontal movement
    character->getBody()->SetLinearVelocity(vel);
}
EnemyStopState::EnemyStopState(Character *_character, int _delay)
    : EnemyState((int)enemyStateType::STOP, _character, _delay)
{
    character->isGrounded = true;
    b2Body *body = character->getBody();
    body->SetLinearVelocity({0, 0});
    b2Filter filter;
    filter.categoryBits = 0;
    filter.maskBits = 0;

    // Iterate over all fixtures attached to this body
    for (b2Fixture *fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
    {
        fixture->SetFilterData(filter);
    }
}

void EnemyStopState::updateState()
{
    State::updateState();
    delayCounter -= GetFrameTime();
    if (frameIndex == numFrames - 1)
    {
        Enemy *enemy = dynamic_cast<Enemy *>(character);
        if (enemy)
        {
            // delete body of enemy
            enemy->beCleared = true;
            cout << "REMOVED\n";
            enemy->needDeletion = true;
        }
    }
}

void EnemyStopState::handleInput()
{
    character->getBody()->SetLinearVelocity({0, 0});
}

// --- FLY STATE IMPLEMENTATION ---

EnemyFlyState::EnemyFlyState(Character *_character)
    : EnemyState((int)enemyStateType::FLY, _character, 5)
{
    // On entering this state, ensure gravity is disabled.
    if (character && character->getBody())
    {
        character->getBody()->SetGravityScale(0.0f);
    }
}

EnemyFlyState::~EnemyFlyState()
{
    // Optional: Restore gravity if the character can stop flying.
    if (character && character->getBody())
    {
        character->getBody()->SetGravityScale(1.0f);
    }
}

void EnemyFlyState::handleInput()
{
}

// --- JUMP STATE IMPLEMENTATION ---

EnemyJumpState::EnemyJumpState(Character *_character, int delay)
    : EnemyState((int)enemyStateType::JUMP, _character, delay)
{
    if (character && character->getBody())
        character->getBody()->SetGravityScale(0.0f);
    initSineJump();
}

EnemyJumpState::~EnemyJumpState()
{
    // restore normal gravity when (if) we ever exit this state
    if (character && character->getBody())
        character->getBody()->SetGravityScale(1.0f);
}

void EnemyJumpState::initSineJump()
{
    elapsed = 0.0f;
    if (character && character->getBody())
        baselineY = character->getBody()->GetPosition().y;
}

void EnemyJumpState::handleInput()
{
    if (!character || !character->getBody())
        return;

    // maintain horizontal patrol speed
    b2Vec2 v = character->getBody()->GetLinearVelocity();
    v.x = -character->getDirection() * horizontalSpeed;
    character->getBody()->SetLinearVelocity(v);
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
    b2Vec2 v = character->getBody()->GetLinearVelocity();
    v.y = vy;
    character->getBody()->SetLinearVelocity(v);

    // (optional) wrap elapsed to avoid large floats
    if (elapsed > period)
        elapsed -= period;
}
