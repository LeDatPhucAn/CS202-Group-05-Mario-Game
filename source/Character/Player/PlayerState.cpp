#include "PlayerState.hpp"
#include "Player.hpp"
#include "Game.hpp"
#include "GameInfo.hpp"
#include "SoundController.hpp"

// Base PlayerState constructor
PlayerState::PlayerState(int Type, Player *_mario, int _delay)
    : State(Type, _mario, _delay), mario(_mario) // Initialize both base and mario pointers
{
    if (Type == (int)playerStateType::JUMP && mario->getForm() == SMALL)
    {
        SoundController::getInstance().playPlayerStateSFX((playerStateType::SMALLJUMP));
    }
    else
        SoundController::getInstance().playPlayerStateSFX((playerStateType)Type);
}
void PlayerState::HorizontalAccelerate(float speedCap, float accel)
{
    float deltaTime = GetFrameTime();
    b2Vec2 vel = mario->body->GetLinearVelocity(); // current velocity
    float currentSpeed = vel.x;
    float speedDiff = speedCap - currentSpeed;

    // Clamp acceleration for this frame (a = Δv / t → Δv = a * t)
    float maxSpeedChange = accel * deltaTime;
    float change = std::clamp(speedDiff, -maxSpeedChange, maxSpeedChange);

    // Apply impulse = mass * Δv
    float impulse = mario->body->GetMass() * change;
    mario->body->ApplyLinearImpulseToCenter(b2Vec2(impulse, 0.0f), true);
}

// ---------- IdleState ----------
IdleState::IdleState(Player *_mario, int _delay)
    : PlayerState((int)playerStateType::IDLE, _mario, _delay)
{
}

void IdleState::handleInput()
{
    if (IsKeyPressed(KEY_ONE))
    {
        mario->changeState(new GrowState(mario));
    }
    else if (IsKeyPressed(KEY_TWO))
    {
        mario->changeState(new UnGrowState(mario));
    }
    else if (IsKeyPressed(mario->getKey(Action::BUTTON_UP)) && mario->isGrounded)
    {
        mario->changeState(new JumpState(mario));
    }
    else if (!mario->isGrounded)
    {
        mario->changeState(new FallState(mario));
    }
    else if (IsKeyDown(mario->getKey(Action::BUTTON_DOWN)) && mario->form != SMALL)
    {
        mario->changeState(new CrouchState(mario));
    }
    else if (IsKeyDown(mario->getKey(Action::BUTTON_RIGHT)) || IsKeyDown(mario->getKey(Action::BUTTON_LEFT)))
    {
        mario->changeState(new WalkState(mario));
    }
    else
    {
        b2Vec2 vel = mario->body->GetLinearVelocity();
        vel.x = 0.0f;
        mario->body->SetLinearVelocity(vel);
    }
}

// ---------- WalkState ----------

WalkState::WalkState(Player *_mario, int _delay)
    : PlayerState((int)playerStateType::WALK, _mario, _delay)
{
}

void WalkState::handleInput()
{
    if (IsKeyPressed(mario->getKey(Action::BUTTON_UP)) && mario->isGrounded)
    {
        mario->changeState(new JumpState(mario));
        return;
    }
    else if (!mario->isGrounded)
    {
        mario->changeState(new FallState(mario));
        return;
    }
    else if (IsKeyPressed(KEY_ONE))
    {
        mario->changeState(new GrowState(mario));
        return;
    }
    else if (IsKeyPressed(KEY_TWO))
    {
        mario->changeState(new UnGrowState(mario));
        return;
    }
    else if (IsKeyDown(mario->getKey(Action::BUTTON_RUN)))
    {
        mario->changeState(new RunState(mario));
        return;
    }
    else if (IsKeyDown(mario->getKey(Action::BUTTON_DOWN)) && mario->form != SMALL)
    {
        mario->changeState(new CrouchState(mario));
        return;
    }

    b2Vec2 vel = mario->body->GetLinearVelocity();

    if (IsKeyDown(mario->getKey(Action::BUTTON_RIGHT)))
    {
        if (mario->direction == LEFT && fabsf(vel.x) > speedTransition)
        {
            mario->changeState(new SkidState(mario));
            return;
        }
        mario->direction = RIGHT;
        HorizontalAccelerate(walkSpeed, walkAccel);
    }
    else if (IsKeyDown(mario->getKey(Action::BUTTON_LEFT)))
    {
        if (mario->direction == RIGHT && fabsf(vel.x) > speedTransition)
        {
            mario->changeState(new SkidState(mario));
            return;
        }
        mario->direction = LEFT;
        HorizontalAccelerate(-walkSpeed, walkAccel);
    }
    else if (fabsf(vel.x) < 0.05f)
    {
        mario->changeState(new IdleState(mario));
        return;
    }
}
// ---------- RunState ----------

RunState::RunState(Player *_mario, int _delay)
    : PlayerState((int)playerStateType::RUN, _mario, _delay)
{
}
void RunState::handleInput()
{
    b2Vec2 vel = mario->body->GetLinearVelocity();

    if (IsKeyPressed(mario->getKey(Action::BUTTON_UP)) && mario->isGrounded)
    {
        mario->changeState(new JumpState(mario));
        return;
    }
    if (!IsKeyDown(mario->getKey(Action::BUTTON_RUN)))
    {
        mario->changeState(new WalkState(mario));
        return;
    }
    if (!mario->isGrounded)
    {
        mario->changeState(new FallState(mario));
        return;
    }
    if (IsKeyDown(mario->getKey(Action::BUTTON_DOWN)) && mario->form != SMALL)
    {
        mario->changeState(new CrouchState(mario));
        return;
    }
    if (IsKeyDown(mario->getKey(Action::BUTTON_RIGHT)))
    {
        if (mario->direction == LEFT && fabsf(vel.x) > speedTransition)
        {
            mario->changeState(new SkidState(mario));
            return;
        }
        mario->direction = RIGHT;
        HorizontalAccelerate(runSpeed, runAccel);
    }
    else if (IsKeyDown(mario->getKey(Action::BUTTON_LEFT)))
    {
        if (mario->direction == RIGHT && fabsf(vel.x) > speedTransition)
        {
            mario->changeState(new SkidState(mario));
            return;
        }
        mario->direction = LEFT;
        HorizontalAccelerate(-runSpeed, runAccel);
    }
    else if (fabsf(vel.x) < 0.05f)
    {
        mario->changeState(new IdleState(mario));
        return;
    }
}

// ---------- JumpState ----------

JumpState::JumpState(Player *_mario, int _delay)
    : PlayerState((int)playerStateType::JUMP, _mario, _delay)
{
    mario->isGrounded = false;
    float mass = mario->body->GetMass();
    b2Vec2 impulse(0, mass * jumpVel);
    mario->body->ApplyLinearImpulseToCenter(impulse, true);
}

void JumpState::handleInput()
{
    b2Vec2 vel = mario->body->GetLinearVelocity();
    float accel = airAccel;
    float speedCap = walkSpeed;
    if (IsKeyDown(mario->getKey(Action::BUTTON_RUN)))
    {
        accel = walkAccel;
        speedCap = runSpeed;
    }

    if (mario->isGrounded)
    {
        if (fabsf(vel.x) < speedTransition)
            mario->changeState(new IdleState(mario));
        else
            mario->changeState(new WalkState(mario));
        return;
    }
    if (vel.y > 0.0f)
    {
        mario->changeState(new FallState(mario));
        return;
    }
    if (IsKeyDown(mario->getKey(Action::BUTTON_RIGHT)))
    {
        mario->direction = RIGHT;
        HorizontalAccelerate(speedCap, accel);
    }
    else if (IsKeyDown(mario->getKey(Action::BUTTON_LEFT)))
    {
        mario->direction = LEFT;
        HorizontalAccelerate(-speedCap, accel);
    }
    else
    {
        float drag = airFriction * GetFrameTime();
        float newVelX = vel.x - copysignf(drag, vel.x);
        if (fabsf(newVelX) < 0.05f / PPM)
            newVelX = 0;
        mario->body->SetLinearVelocity({newVelX, vel.y});
    }
}

// ---------- FallState ----------
FallState::FallState(Player *_mario, int _delay)
    : PlayerState((int)playerStateType::FALL, _mario, _delay)
{
}

void FallState::handleInput()
{
    b2Vec2 vel = mario->body->GetLinearVelocity();
    float accel = airAccel;
    float speedCap = walkSpeed;

    if (IsKeyDown(mario->getKey(Action::BUTTON_RUN)))
    {
        accel = walkAccel;
        speedCap = runSpeed;
    }
    if (mario->isGrounded)
    {
        if (fabsf(vel.x) < speedTransition)
            mario->changeState(new IdleState(mario));
        else
            mario->changeState(new WalkState(mario));
        return;
    }
    if (IsKeyDown(mario->getKey(Action::BUTTON_RIGHT)))
    {
        mario->direction = RIGHT;
        HorizontalAccelerate(speedCap, accel);
    }
    else if (IsKeyDown(mario->getKey(Action::BUTTON_LEFT)))
    {
        mario->direction = LEFT;
        HorizontalAccelerate(-speedCap, accel);
    }
    else
    {
        float drag = airFriction * GetFrameTime();
        float newVelX = vel.x - copysignf(drag, vel.x);
        if (fabsf(newVelX) < 0.05f / PPM)
            newVelX = 0;
        mario->body->SetLinearVelocity({newVelX, vel.y});
    }
}
// ---------- SkidState ----------

SkidState::SkidState(Player *_mario, int _delay)
    : PlayerState((int)playerStateType::SKID, _mario, _delay)
{
}

void SkidState::handleInput()
{
    b2Vec2 vel = mario->body->GetLinearVelocity();
    if (fabs(vel.x) < 0.6f)
    {
        mario->changeState(new IdleState(mario));
        return;
    }
    else if (IsKeyPressed(mario->getKey(Action::BUTTON_UP)) && mario->isGrounded)
    {
        mario->changeState(new JumpState(mario));
        return;
    }

    float accel = -skidDecel;
    if (vel.x < 0)
        accel = skidDecel;

    if (IsKeyDown(mario->getKey(Action::BUTTON_RIGHT)))
    {
        mario->direction = RIGHT;
    }
    else if (IsKeyDown(mario->getKey(Action::BUTTON_LEFT)))
    {
        mario->direction = LEFT;
    }
    else
    {
        accel = (vel.x > 0) ? -friction * 2 : friction * 2;
    }

    float deltaTime = GetFrameTime();
    float newVelX = vel.x + accel * deltaTime;
    mario->body->SetLinearVelocity({newVelX, vel.y});
}

// ---------- CrouchState ----------
CrouchState::CrouchState(Player *_mario, int _delay)
    : PlayerState((int)playerStateType::CROUCH, _mario, _delay)
{
    mario->toNewBody();
}
void CrouchState::handleInput()
{
    b2Vec2 vel = mario->body->GetLinearVelocity();
    float accel = mario->direction * (-friction);

    if (!IsKeyDown(mario->getKey(Action::BUTTON_DOWN)))
    {
        mario->toNewBody(); // Reset to normal body size
        if (!mario->isGrounded)
            mario->changeState(new FallState(mario));
        else if (abs(vel.x) < 0.6f)
            mario->changeState(new IdleState(mario));
        else
            mario->changeState(new WalkState(mario));
        return;
    }
    if (IsKeyPressed(mario->getKey(Action::BUTTON_UP)) && mario->isGrounded)
    {
        // mario->changeState(new JumpState(mario));
        mario->isGrounded = false;
        float mass = mario->body->GetMass();
        b2Vec2 impulse(0, mass * jumpVel);
        mario->body->ApplyLinearImpulseToCenter(impulse, true);
        return;
    }
    if (abs(vel.x) < 0.6f)
    {
        mario->body->SetLinearVelocity({0, vel.y});
        return;
    }
    float deltaTime = GetFrameTime();
    float newVelX = vel.x + accel * deltaTime;
    mario->body->SetLinearVelocity({newVelX, vel.y});
}

// ---------- GrowState ----------

GrowState::GrowState(Player *_mario, int _delay)
    : PlayerState((int)playerStateType::GROW, _mario, _delay)
{
    posBeforeY = mario->getPosition().y;
    mario->turnInvincible();
}

void GrowState::handleInput()
{
    if (mario->form == FIRE)
    {
        mario->changeState(new IdleState(mario));
        return;
    }
    StartEndFrame se = mario->sprite.StartEndFrames[type];

    if (delayCounter == 0)
    {
        float thisHeight = mario->sprite.frameRecs[frameIndex + se.start - 1].height;
        float nextHeight = mario->sprite.frameRecs[frameIndex + se.start].height;
        float dif = nextHeight - thisHeight;

        posBeforeY -= dif;
    }
    mario->body->SetLinearVelocity({0, 0});
    Vector2 pos = mario->getPosition();
    mario->setPosition({pos.x, posBeforeY});

    if (se.start + frameIndex == se.end)
    {

        mario->form = static_cast<PlayerForm>((mario->form + 1) % FORM_COUNT);
        mario->changeForm(mario->form);

        // new collision box
        mario->toNewBody();

        if (mario->isGrounded)
        {
            mario->changeState(new IdleState(mario));
        }
        else
        {
            mario->changeState(new FallState(mario));
        }

        return;
    }
}
UnGrowState::UnGrowState(Player *_mario, int _delay)
    : PlayerState((int)playerStateType::UNGROW, _mario, _delay)
{
    posBeforeY = mario->getPosition().y;
}

void UnGrowState::handleInput()
{
    if (mario->form == SMALL)
    {
        mario->changeState(new DeadState(mario));
        return;
    }
    StartEndFrame se = mario->sprite.StartEndFrames[type];

    if (delayCounter == 0)
    {
        float thisHeight = mario->sprite.frameRecs[-frameIndex + se.start + 1].height;
        float nextHeight = mario->sprite.frameRecs[-frameIndex + se.start].height;
        float dif = nextHeight - thisHeight;

        posBeforeY -= dif;
    }
    mario->body->SetLinearVelocity({0, 0});
    Vector2 pos = mario->getPosition();
    mario->setPosition({pos.x, posBeforeY});
    if (se.start - frameIndex == se.end)
    {
        mario->form = static_cast<PlayerForm>((mario->form - 1 + FORM_COUNT) % FORM_COUNT);
        mario->changeForm(mario->form);
        // new collision box
        mario->toNewBody();
        if (mario->isGrounded)
        {
            mario->changeState(new IdleState(mario));
        }
        else
        {
            mario->changeState(new FallState(mario));
        }
        return;
    }
}

DeadState::DeadState(Player *_mario, int _delay)
    : PlayerState((int)playerStateType::DEAD, _mario, _delay)
{
    SoundController::getInstance().stopSceneMusic();
    mario->isGrounded = false;
    mario->body->SetLinearVelocity({0, -250.f / PPM});
    b2Body *body = mario->getBody();
    b2Filter filter;
    filter.categoryBits = 0;
    filter.maskBits = 0;

    // Iterate over all fixtures attached to this body
    for (b2Fixture *fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
    {
        fixture->SetFilterData(filter);
    }
}

void DeadState::handleInput()
{
    // No need for repspawing anymore
    //     if (IsKeyPressed(KEY_R))
    //     {
    //         mario->toNewBody();
    //         mario->changeState(new FallState(mario));
    //         return;
    //     }
}
