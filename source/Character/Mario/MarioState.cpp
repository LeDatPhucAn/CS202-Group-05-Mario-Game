#include "MarioState.hpp"
#include "Mario.hpp"
#include "Game.hpp"
#include "GrowMushroom.hpp"
// Base MarioState constructor
MarioState::MarioState(int Type, Mario *_mario, int _delay)
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
    : MarioState((int)marioStateType::IDLE, _mario, _delay)
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
    else if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && mario->isGrounded)
    {
        mario->changeState(new JumpState(mario));
    }
    else if (!mario->isGrounded)
    {
        mario->changeState(new FallState(mario));
    }
    else if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) && mario->form != SMALL)
    {
        mario->changeState(new CrouchState(mario));
    }
    else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D) || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    {
        mario->changeState(new WalkState(mario));
    }
    else if ((IsKeyDown(KEY_M)))
    {
        GrowMushroom *newMushroom = new GrowMushroom();
        newMushroom->test_appear();
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
    : MarioState((int)marioStateType::WALK, _mario, _delay)
{
}

void WalkState::handleInput()
{
    if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && mario->isGrounded)
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
    else if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) && mario->form != SMALL)
    {
        mario->changeState(new CrouchState(mario));
        return;
    }

    b2Vec2 vel = mario->body->GetLinearVelocity();

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {
        if (mario->direction == LEFT && fabsf(vel.x) > 0.6f)
        {
            mario->changeState(new SkidState(mario));
            return;
        }
        mario->direction = RIGHT;
        HorizontalAccelerate(walkSpeed, walkAccel);
    }
    else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
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
    : MarioState((int)marioStateType::RUN, _mario, _delay)
{
}
void RunState::handleInput()
{
    b2Vec2 vel = mario->body->GetLinearVelocity();

    if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && mario->isGrounded)
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
    if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) && mario->form != SMALL)
    {
        mario->changeState(new CrouchState(mario));
        return;
    }
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {
        if (mario->direction == LEFT && fabsf(vel.x) > 0.6f)
        {
            mario->changeState(new SkidState(mario));
            return;
        }
        mario->direction = RIGHT;
        HorizontalAccelerate(runSpeed, runAccel);
    }
    else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
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
    : MarioState((int)marioStateType::JUMP, _mario, _delay)
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
        if (fabsf(vel.x) < 0.6f)
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
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {
        mario->direction = RIGHT;
        HorizontalAccelerate(speedCap, accel);
    }
    else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    {
        mario->direction = LEFT;
        HorizontalAccelerate(-speedCap, accel);
    }
    else
    {
        float drag = airFriction * GetFrameTime();
        float newVelX = vel.x - copysignf(drag, vel.x);
        if (fabsf(newVelX) < 0.05f)
            newVelX = 0;
        mario->body->SetLinearVelocity({newVelX, vel.y});
    }
}

// ---------- FallState ----------
FallState::FallState(Mario *_mario, int _delay)
    : MarioState((int)marioStateType::FALL, _mario, _delay)
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
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {
        mario->direction = RIGHT;
        HorizontalAccelerate(speedCap, accel);
    }
    else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    {
        mario->direction = LEFT;
        HorizontalAccelerate(-speedCap, accel);
    }
    else
    {
        float drag = airFriction * GetFrameTime();
        float newVelX = vel.x - copysignf(drag, vel.x);
        if (fabsf(newVelX) < 0.05f)
            newVelX = 0;
        mario->body->SetLinearVelocity({newVelX, vel.y});
    }
}
// ---------- SkidState ----------

SkidState::SkidState(Mario *_mario, int _delay)
    : MarioState((int)marioStateType::SKID, _mario, _delay)
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
    else if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && mario->isGrounded)
    {
        mario->changeState(new JumpState(mario));
        return;
    }

    float accel = -skidDecel;
    if (vel.x < 0)
        accel = skidDecel;

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {
        mario->direction = RIGHT;
    }
    else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
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
    : MarioState((int)marioStateType::CROUCH, _mario, _delay)
{
    mario->toNewBody();
}
void CrouchState::handleInput()
{
    b2Vec2 vel = mario->body->GetLinearVelocity();
    float accel = mario->direction * (-friction);

    if (!(IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)))
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
    if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && mario->isGrounded)
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

GrowState::GrowState(Mario *_mario, int _delay)
    : MarioState((int)marioStateType::GROW, _mario, _delay)
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
    // new collision box
    mario->toNewBody();
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
    : MarioState((int)marioStateType::UNGROW, _mario, _delay)
{
    // if (mario->isGrounded)
    //     mario->pos.y = mario->groundPosY - frameRec.height;
}

void UnGrowState::handleInput()
{
    StartEndFrame se = mario->sprite.StartEndFrames[type];
    if (mario->form == SMALL)
    {
        mario->changeState(new DeadState(mario));
        return;
    }
    // new collision box
    mario->toNewBody();
    if (se.start - frameIndex == se.end)
    {
        mario->form = static_cast<MarioForm>((mario->form - 1 + FORM_COUNT) % FORM_COUNT);
        mario->changeForm(mario->form);

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

DeadState::DeadState(Mario *_mario, int _delay)
    : MarioState((int)marioStateType::DEAD, _mario, _delay)
{
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
    if (IsKeyPressed(KEY_R))
    {
        mario->toNewBody();
        mario->changeState(new FallState(mario));
        return;
    }
}

ThrowFBState::ThrowFBState(Mario *_mario, int _delay)
    : MarioState((int)marioStateType::THROWFB, _mario, _delay) {}

void ThrowFBState::handleInput()
{
    if (delayCounter == delay)
    {
        mario->throwFireBall();

        mario->changeState(new WalkState(mario));
    }
}