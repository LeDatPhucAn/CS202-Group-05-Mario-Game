#include "MovingObjectState.hpp"
#include "MovingObject.hpp"
#include "Character.hpp"
#include "Game.hpp"
#include "GameInfo.hpp"
#include "SoundController.hpp"
MovingObjectState::MovingObjectState(int type, Character *_character, int _delay)
    : State(type, _character, _delay), character(_character) {}

MovingObjectIdleState::MovingObjectIdleState(Character *_character, int _delay)
    : MovingObjectState((int)movingObjectStateType::IDLE, _character, _delay)
{
}

void MovingObjectIdleState::handleInput()
{
    b2Vec2 vel = character->getBody()->GetLinearVelocity();
    vel.x = 0.0f;
    character->getBody()->SetLinearVelocity(vel);
}

MovingObjectWalkState::MovingObjectWalkState(Character *_character, int _delay)
    : MovingObjectState((int)movingObjectStateType::WALK, _character, _delay)
{
    character->direction = RIGHT;
}

void MovingObjectWalkState::handleInput()
{
    b2Vec2 vel = character->getBody()->GetLinearVelocity();
    vel.x = -character->direction * fabs(20 / PPM); // Ensure correct direction
    character->getBody()->SetLinearVelocity(vel);
}

MovingObjectRunState::MovingObjectRunState(Character *_character, int _delay)
    : MovingObjectState((int)movingObjectStateType::RUN, _character, _delay)
{
}

void MovingObjectRunState::handleInput()
{
    b2Vec2 vel = character->getBody()->GetLinearVelocity();
    vel.x = -character->direction * fabs(200 / PPM); // Ensure correct direction
    character->getBody()->SetLinearVelocity(vel);
}
FireBallMoveState::FireBallMoveState(Character *_character, int _delay)
    : MovingObjectState((int)movingObjectStateType::FB_MOVE, _character, _delay)
{
}

void FireBallMoveState::handleInput()
{
    b2Body *body = character->getBody();
    b2Vec2 vel = body->GetLinearVelocity();
    vel.x = -character->getDirection() * fabs(200 / PPM);
    body->SetLinearVelocity(vel);
    if (character->isGrounded)
    {
        float mass = body->GetMass();
        b2Vec2 impulse(0, mass * jumpVel / 4);
        body->ApplyLinearImpulseToCenter(impulse, true);
    }
}
LuigiFireBallMoveState::LuigiFireBallMoveState(Character *_character, int _delay)
    : MovingObjectState((int)movingObjectStateType::LUIGI_FB_MOVE, _character, _delay)
{
}

void LuigiFireBallMoveState::handleInput()
{
    b2Body *body = character->getBody();
    b2Vec2 vel = body->GetLinearVelocity();
    vel.x = -character->getDirection() * fabs(200 / PPM);
    body->SetLinearVelocity(vel);
    if (!character->isGrounded)
    {
        float gravityAccel = (character->getBody()->GetLinearVelocity().y < 0 && (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)))
                                 ? jumpGravity
                                 : addedFallGravity;
        character->getBody()->ApplyForceToCenter({0, -character->getBody()->GetMass() * gravityAccel}, true);
    }
}
StarMoveState::StarMoveState(Character *_character, int _delay)
    : MovingObjectState((int)movingObjectStateType::STAR_MOVE, _character, _delay)
{
}

void StarMoveState::handleInput()
{
    b2Body *body = character->getBody();
    b2Vec2 vel = body->GetLinearVelocity();

    // speed is 50 / PPM
    vel.x = -character->getDirection() * fabs(50 / PPM);

    body->SetLinearVelocity(vel);
    if (character->isGrounded)
    {
        float mass = body->GetMass();
        b2Vec2 impulse(0, mass * jumpVel / 4);
        body->ApplyLinearImpulseToCenter(impulse, true);
    }
}

MovingObjectDeadState::MovingObjectDeadState(Character *_character, int _delay)
    : MovingObjectState((int)movingObjectStateType::DEAD, _character, _delay)
{
    // make a sound when an MovingObject dies
    SoundController::getInstance().playPlayerStateSFX(playerStateType::SQUISH_ENEMY);
    GameInfo::getInstance()->addScore(100); // Add score for defeating MovingObject
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

void MovingObjectDeadState::updateState()
{
    State::updateState();
    delayCounter -= GetFrameTime();
    if (delayCounter <= 0)
    {
        MovingObject *movingObject = dynamic_cast<MovingObject *>(character);
        if (movingObject)
        {
            // delete body of MovingObject
            movingObject->beCleared = true;
            cout << "REMOVED\n";
            movingObject->needDeletion = true;
        }
    }
}
void MovingObjectDeadState::handleInput()
{
    b2Vec2 vel = character->getBody()->GetLinearVelocity();
    vel.x = 0.0f; // Ensure no horizontal movement
    character->getBody()->SetLinearVelocity(vel);
}
MovingObjectStopState::MovingObjectStopState(Character *_character, int _delay)
    : MovingObjectState((int)movingObjectStateType::STOP, _character, _delay)
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

void MovingObjectStopState::updateState()
{
    State::updateState();
    delayCounter -= GetFrameTime();
    if (frameIndex == numFrames - 1)
    {
        MovingObject *movingObject = dynamic_cast<MovingObject *>(character);
        if (movingObject)
        {
            // delete body of MovingObject
            movingObject->beCleared = true;
            cout << "REMOVED\n";
            movingObject->needDeletion = true;
        }
    }
}

void MovingObjectStopState::handleInput()
{
    character->getBody()->SetLinearVelocity({0, 0});
}

// --- FLY STATE IMPLEMENTATION ---

MovingObjectFlyState::MovingObjectFlyState(Character *_character)
    : MovingObjectState((int)movingObjectStateType::FLY, _character, 5)
{
    // On entering this state, ensure gravity is disabled.
    if (character && character->getBody())
    {
        character->getBody()->SetGravityScale(0.0f);
    }
}

MovingObjectFlyState::~MovingObjectFlyState()
{
    // Optional: Restore gravity if the character can stop flying.
    if (character && character->getBody())
    {
        character->getBody()->SetGravityScale(1.0f);
    }
}

void MovingObjectFlyState::handleInput()
{
}

// --- JUMP STATE IMPLEMENTATION ---

MovingObjectJumpState::MovingObjectJumpState(Character *_character, int delay)
    : MovingObjectState((int)movingObjectStateType::JUMP, _character, delay)
{
    if (character && character->getBody())
        character->getBody()->SetGravityScale(0.0f);
    initSineJump();
}

MovingObjectJumpState::~MovingObjectJumpState()
{
    // restore normal gravity when (if) we ever exit this state
    if (character && character->getBody())
        character->getBody()->SetGravityScale(1.0f);
}

void MovingObjectJumpState::initSineJump()
{
    elapsed = 0.0f;
    if (character && character->getBody())
        baselineY = character->getBody()->GetPosition().y;
}

void MovingObjectJumpState::handleInput()
{
    if (!character || !character->getBody())
        return;

    // maintain horizontal patrol speed
    b2Vec2 v = character->getBody()->GetLinearVelocity();
    v.x = -character->getDirection() * horizontalSpeed;
    character->getBody()->SetLinearVelocity(v);
}

void MovingObjectJumpState::updateState()
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

// --- THROW STATE IMPLEMENTATION ---

MovingObjectThrowState::MovingObjectThrowState(Character *character, int delay)
    : MovingObjectState((int)movingObjectStateType::THROW, character, delay)
{
    throwTimer = 0.0f;
    hasThrown = false;
}

void MovingObjectThrowState::handleInput()
{
    // Slow down or stop horizontal movement during throw
    b2Vec2 vel = character->getBody()->GetLinearVelocity();
    vel.x *= 0.5f; // Reduce speed by half during throw
    character->getBody()->SetLinearVelocity(vel);
}

void MovingObjectThrowState::updateState()
{
    State::updateState();

    throwTimer += GetFrameTime();

    // Execute throw at mid-point of animation
    if (!hasThrown && throwTimer >= throwDuration * 0.5f)
    {
        executeThrow();
        hasThrown = true;
    }

    // Return to previous state after throw duration
    if (throwTimer >= throwDuration)
    {
        // Return to flying state (or appropriate state for the MovingObject)
        character->changeState(new MovingObjectFlyState(character));
    }
}
