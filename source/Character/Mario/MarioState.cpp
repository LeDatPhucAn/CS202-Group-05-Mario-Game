#include "MarioState.hpp"
#include "Mario.hpp"

// Base MarioState constructor
MarioState::MarioState(stateType Type, Mario *_mario, int _delay)
    : State(Type, _mario, _delay), mario(_mario) // Initialize both base and mario pointers
{
}
void MarioState::HorizontalAccelerate(float speedCap, float accel)
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
IdleState::IdleState(Mario *_mario, int _delay)
    : MarioState(IDLE, _mario, _delay)
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
    else if (IsKeyPressed(KEY_UP) && mario->isGrounded)
    {
        mario->changeState(new JumpState(mario));
    }
    else if (!mario->isGrounded)
    {
        mario->changeState(new FallState(mario));
    }
    else if (IsKeyDown(KEY_DOWN) && mario->form != SMALL)
    {
        mario->changeState(new CrouchState(mario));
    }
    else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT))
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

WalkState::WalkState(Mario *_mario, int _delay)
    : MarioState(WALK, _mario, _delay)
{
}

void WalkState::handleInput()
{

    if (IsKeyPressed(KEY_UP) && mario->isGrounded)
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
    else if (IsKeyDown(KEY_LEFT_CONTROL))
    {
        mario->changeState(new RunState(mario));
        return;
    }
    else if (IsKeyDown(KEY_DOWN) && mario->form != SMALL)
    {
        mario->changeState(new CrouchState(mario));
        return;
    }

    b2Vec2 vel = mario->body->GetLinearVelocity(); // current velocity

    // Handle left/right input using velocity
    if (IsKeyDown(KEY_RIGHT))
    {
        if (mario->direction == LEFT && fabsf(vel.x) > 0.6f) // ~20 px/frame
        {
            mario->changeState(new SkidState(mario));
            return;
        }

        mario->direction = RIGHT;

        HorizontalAccelerate(walkSpeed, walkAccel);
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        if (mario->direction == RIGHT && fabsf(vel.x) > 0.6f)
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

RunState::RunState(Mario *_mario, int _delay)
    : MarioState(RUN, _mario, _delay)
{
}

void RunState::handleInput()
{
    b2Vec2 vel = mario->body->GetLinearVelocity();

    if (IsKeyPressed(KEY_UP) && mario->isGrounded)
    {
        mario->changeState(new JumpState(mario));
        return;
    }

    if (!IsKeyDown(KEY_LEFT_CONTROL))
    {
        mario->changeState(new WalkState(mario));
        return;
    }

    if (!mario->isGrounded)
    {
        mario->changeState(new FallState(mario));
        return;
    }

    if (IsKeyDown(KEY_DOWN) && mario->form != SMALL)
    {
        mario->changeState(new CrouchState(mario));
        return;
    }

    if (IsKeyDown(KEY_RIGHT))
    {
        if (mario->direction == LEFT && fabsf(vel.x) > 0.6f)
        {
            mario->changeState(new SkidState(mario));
            return;
        }

        mario->direction = RIGHT;
        HorizontalAccelerate(runSpeed, runAccel);
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        if (mario->direction == RIGHT && fabsf(vel.x) > 0.6f)
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

JumpState::JumpState(Mario *_mario, int _delay)
    : MarioState(JUMP, _mario, _delay)
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
    if (IsKeyDown(KEY_LEFT_CONTROL))
    {
        accel = walkAccel;
        speedCap = runSpeed;
    }

    if (mario->isGrounded)
    {
        // Landed
        if (fabsf(vel.x) < 0.6f)
            mario->changeState(new IdleState(mario));
        else
            mario->changeState(new WalkState(mario));
        return;
    }

    // Transition to Fall if upward velocity has ended
    if (vel.y > 0.0f)
    {
        mario->changeState(new FallState(mario));
        return;
    }

    // Horizontal movement in-air
    if (IsKeyDown(KEY_RIGHT))
    {
        mario->direction = RIGHT;
        HorizontalAccelerate(speedCap, accel);
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        mario->direction = LEFT;
        HorizontalAccelerate(-speedCap, accel);
    }
    else
    {
        // No input → light air drag
        float drag = airFriction * GetFrameTime();
        float newVelX = vel.x - copysignf(drag, vel.x);
        if (fabsf(newVelX) < 0.05f)
            newVelX = 0;
        mario->body->SetLinearVelocity({newVelX, vel.y});
    }
}

// ---------- FallState ----------
FallState::FallState(Mario *_mario, int _delay)
    : MarioState(FALL, _mario, _delay)
{
}

void FallState::handleInput()
{
    b2Vec2 vel = mario->body->GetLinearVelocity();
    float accel = airAccel;
    float speedCap = walkSpeed;

    if (IsKeyDown(KEY_LEFT_CONTROL))
    {
        accel = walkAccel;
        speedCap = runSpeed;
    }

    if (mario->isGrounded)
    {
        if (fabsf(vel.x) < 0.6f)
            mario->changeState(new IdleState(mario));
        else
            mario->changeState(new WalkState(mario));
        return;
    }

    if (IsKeyDown(KEY_RIGHT))
    {
        mario->direction = RIGHT;
        HorizontalAccelerate(speedCap, accel);
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        mario->direction = LEFT;
        HorizontalAccelerate(-speedCap, accel);
    }
    else
    {
        // No input → light air drag
        float drag = airFriction * GetFrameTime();
        float newVelX = vel.x - copysignf(drag, vel.x);
        if (fabsf(newVelX) < 0.05f)
            newVelX = 0;
        mario->body->SetLinearVelocity({newVelX, vel.y});
    }
}
// ---------- SkidState ----------

SkidState::SkidState(Mario *_mario, int _delay)
    : MarioState(SKID, _mario, _delay)
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
    else if (IsKeyPressed(KEY_UP) && mario->isGrounded)
    {
        mario->changeState(new JumpState(mario));
        return;
    }

    float accel = -skidDecel;
    if (vel.x < 0)
        accel = skidDecel;

    if (IsKeyDown(KEY_RIGHT))
    {
        mario->direction = RIGHT;
    }
    else if (IsKeyDown(KEY_LEFT))
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
CrouchState::CrouchState(Mario *_mario, int _delay)
    : MarioState(CROUCH, _mario, _delay)
{
}
void CrouchState::handleInput()
{
    b2Vec2 vel = mario->body->GetLinearVelocity();

    float accel = mario->direction * (-friction);

    if (!IsKeyDown(KEY_DOWN))
    {
        if (abs(vel.x) < 0.6f)
            mario->changeState(new IdleState(mario));
        else
            mario->changeState(new WalkState(mario));
        return;
    }
    if (IsKeyPressed(KEY_UP))
    {
        mario->changeState(new JumpState(mario));
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

GrowState::GrowState(Mario *_mario, int _delay)
    : MarioState(GROW, _mario, _delay)
{
    // if (mario->isGrounded)
    //     mario->pos.y = mario->groundPosY - frameRec.height;
}

void GrowState::handleInput()
{
    if (mario->form == FIRE)
    {
        mario->changeState(new IdleState(mario));
        return;
    }
    StartEndFrame se = mario->sprite.StartEndFrames[type];

    cout << frameIndex << " " << frameRec.height << boolalpha << mario->isGrounded << "\n";
    if (se.start + frameIndex == se.end)
    {
        mario->form = static_cast<MarioForm>((mario->form + 1) % FORM_COUNT);
        mario->changeForm(mario->form);
        if (mario->isGrounded)
        {
            // mario->pos.y = mario->groundPosY - frameRec.height;
            mario->changeState(new IdleState(mario));
        }
        else
        {
            mario->changeState(new FallState(mario));
        }
        return;
    }
}
UnGrowState::UnGrowState(Mario *_mario, int _delay)
    : MarioState(UNGROW, _mario, _delay)
{
    // if (mario->isGrounded)
    //     mario->pos.y = mario->groundPosY - frameRec.height;
}

void UnGrowState::handleInput()
{
    StartEndFrame se = mario->sprite.StartEndFrames[type];
    if (mario->form == SMALL)
    {
        mario->changeState(new IdleState(mario));
        return;
    }
    // if (mario->isGrounded)
    //     mario->pos.y = mario->groundPosY - frameRec.height;

    if (se.start - frameIndex == se.end)
    {
        mario->form = static_cast<MarioForm>((mario->form - 1 + FORM_COUNT) % FORM_COUNT);
        mario->changeForm(mario->form);
        if (mario->isGrounded)
        {
            // mario->pos.y = mario->groundPosY - frameRec.height;
            mario->changeState(new IdleState(mario));
        }
        else
        {
            mario->changeState(new FallState(mario));
        }
        return;
    }
}

DeadState::DeadState(Mario *_mario, int _delay)
    : MarioState(DEAD, _mario, _delay)
{
    mario->isGrounded = false;
    mario->body->SetLinearVelocity({0, -250.f / PPM});
}

void DeadState::handleInput()
{
    if (IsKeyPressed(KEY_R))
    {
        // Reset character
        mario->pos = {100, 0};
        mario->changeState(new FallState(mario));
        return;
    }
}