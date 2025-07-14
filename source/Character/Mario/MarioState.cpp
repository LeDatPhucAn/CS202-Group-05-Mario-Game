#include "MarioState.hpp"
#include "Mario.hpp"

// Base MarioState constructor
MarioState::MarioState(stateType Type, Mario *_mario, int _delay)
    : State(Type, _mario, _delay), mario(_mario) // Initialize both base and mario pointers
{
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
        mario->movement.velocity.x = 0;
        mario->movement.acceleration.x = 0;
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
    if (IsKeyDown(KEY_RIGHT))
    {
        if (mario->direction == LEFT && abs(mario->movement.velocity.x) > 20.0f)
        {
            mario->changeState(new SkidState(mario));
            return;
        }
        mario->direction = RIGHT;
        mario->movement.acceleration.x = walkAccel;
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        if (mario->direction == RIGHT && abs(mario->movement.velocity.x) > 20.0f)
        {
            mario->changeState(new SkidState(mario));
            return;
        }
        mario->direction = LEFT;
        mario->movement.acceleration.x = -walkAccel;
    }
    else
    {
        mario->movement.acceleration.x = mario->direction * (-friction);

        if (abs(mario->movement.velocity.x) < 20.0f)
        {
            mario->changeState(new IdleState(mario));
            return;
        }
    }
    // Clamp walk speed
    if (abs(mario->movement.velocity.x) > walkSpeed)
    {
        mario->movement.velocity.x = walkSpeed * mario->direction;
    }
}

// ---------- JumpState ----------

JumpState::JumpState(Mario *_mario, int _delay)
    : MarioState(JUMP, _mario, _delay)
{
    mario->isGrounded = false;
    mario->movement.velocity.y = jumpVel;
}

void JumpState::handleInput()
{
    float accel = airAccel;
    if (IsKeyDown(KEY_LEFT_CONTROL))
    {
        accel = walkAccel;
    }
    if (mario->isGrounded)
    {
        if (abs(mario->movement.velocity.x) < 20.0f)
        {
            mario->changeState(new IdleState(mario));
        }
        else
        {
            mario->changeState(new WalkState(mario));
        }
        return;
    }
    if (mario->movement.velocity.y > 0)
    {
        mario->changeState(new FallState(mario));
        return;
    }

    if (IsKeyDown(KEY_RIGHT))
    {
        mario->direction = RIGHT;
        mario->movement.acceleration.x = accel;
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        mario->direction = LEFT;
        mario->movement.acceleration.x = -accel;
    }
    else
    {
        mario->movement.acceleration.x = mario->direction * accel / 8;
    }
}

// ---------- FallState ----------
FallState::FallState(Mario *_mario, int _delay)
    : MarioState(FALL, _mario, _delay)
{
}

void FallState::handleInput()
{
    // float airSpeed = walkSpeed;
    float accel = airAccel;
    if (IsKeyDown(KEY_LEFT_CONTROL))
    {
        accel = walkAccel;
        // airSpeed = runSpeed;
    }
    if (mario->isGrounded)
    {
        if (abs(mario->movement.velocity.x) < 20)
        {
            mario->changeState(new IdleState(mario));
            return;
        }
        else
        {
            if (mario->movement.velocity.x * mario->direction < 0)
            {
                mario->direction = (mario->direction == LEFT) ? RIGHT : LEFT;
            }
            mario->changeState(new WalkState(mario));
            return;
        }
    }

    if (IsKeyDown(KEY_RIGHT))
    {
        mario->direction = RIGHT;
        mario->movement.acceleration.x = accel;
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        mario->direction = LEFT;
        mario->movement.acceleration.x = -accel;
    }
    else
    {
        mario->movement.acceleration.x = mario->direction * (-airFriction);

        if (abs(mario->movement.velocity.x) < 20.0f)
        {
            mario->movement.velocity.x = 0;
            mario->movement.acceleration.x = 0;
            return;
        }
    }
    // // Clamp air speed
    // if (abs(mario->movement.velocity.x) > airSpeed)
    // {
    //     mario->movement.velocity.x = airSpeed * mario->direction;
    // }
}

// ---------- SkidState ----------

SkidState::SkidState(Mario *_mario, int _delay)
    : MarioState(SKID, _mario, _delay)
{
}

void SkidState::handleInput()
{
    if (abs(mario->movement.velocity.x) < 20)
    {
        mario->changeState(new IdleState(mario));
        return;
    }
    else if (IsKeyPressed(KEY_UP) && mario->isGrounded)
    {
        mario->changeState(new JumpState(mario));
        return;
    }

    if (IsKeyDown(KEY_RIGHT))
    {
        mario->direction = RIGHT;
        mario->movement.acceleration.x = (mario->movement.velocity.x > 0) ? -skidDecel : skidDecel;
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        mario->direction = LEFT;
        mario->movement.acceleration.x = (mario->movement.velocity.x > 0) ? -skidDecel : skidDecel;
    }
    else
    {

        mario->movement.acceleration.x = (mario->movement.velocity.x > 0) ? -friction * 2 : friction * 2;
    }
}

// ---------- RunState ----------

RunState::RunState(Mario *_mario, int _delay)
    : MarioState(RUN, _mario, _delay)
{
}

void RunState::handleInput()
{
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
        if (mario->direction == LEFT && abs(mario->movement.velocity.x) > 20.0f)
        {
            mario->changeState(new SkidState(mario));
            return;
        }
        mario->direction = RIGHT;
        mario->movement.acceleration.x = runAccel;
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        if (mario->direction == RIGHT && abs(mario->movement.velocity.x) > 20.0f)
        {
            mario->changeState(new SkidState(mario));
            return;
        }
        mario->direction = LEFT;
        mario->movement.acceleration.x = -runAccel;
    }
    else
    {
        mario->movement.acceleration.x = mario->direction * (-friction);

        if (abs(mario->movement.velocity.x) < 20)
        {
            mario->changeState(new IdleState(mario));
            return;
        }
    }

    // Clamp run speed
    if (abs(mario->movement.velocity.x) > runSpeed)
    {
        mario->movement.velocity.x = runSpeed * mario->direction;
    }
}

// ---------- CrouchState ----------
CrouchState::CrouchState(Mario *_mario, int _delay)
    : MarioState(CROUCH, _mario, _delay)
{
}
void CrouchState::handleInput()
{
    mario->movement.acceleration.x = mario->direction * (-friction);

    if (!IsKeyDown(KEY_DOWN))
    {
        if (abs(mario->movement.velocity.x) < 20.0f)
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
    if (abs(mario->movement.velocity.x) < 20.0f)
    {
        mario->movement.velocity.x = 0;
        mario->movement.acceleration.x = 0;
        return;
    }
}

// ---------- GrowState ----------

GrowState::GrowState(Mario *_mario, int _delay)
    : MarioState(GROW, _mario, _delay)
{
    if (mario->isGrounded)
        mario->pos.y = mario->groundPosY - frameRec.height;
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
            mario->pos.y = mario->groundPosY - frameRec.height;
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
    if (mario->isGrounded)
        mario->pos.y = mario->groundPosY - frameRec.height;
}

void UnGrowState::handleInput()
{
    StartEndFrame se = mario->sprite.StartEndFrames[type];
    if (mario->form == SMALL)
    {
        mario->changeState(new IdleState(mario));
        return;
    }
    if (mario->isGrounded)
        mario->pos.y = mario->groundPosY - frameRec.height;

    if (se.start - frameIndex == se.end)
    {
        mario->form = static_cast<MarioForm>((mario->form - 1 + FORM_COUNT) % FORM_COUNT);
        mario->changeForm(mario->form);
        if (mario->isGrounded)
        {
            mario->pos.y = mario->groundPosY - frameRec.height;
            mario->changeState(new IdleState(mario));
        }
        else
        {
            mario->changeState(new FallState(mario));
        }
        return;
    }
}


DeadState::DeadState(Mario* _mario, int _delay)
    : MarioState(DEAD, _mario, _delay)
{   
    mario->isGrounded = false;
    mario->movement.velocity.y = -250.f;
    mario->movement.velocity.x = 0;
    mario->movement.acceleration.x = 0;
}

void DeadState::handleInput()
{
    if (IsKeyPressed(KEY_R))
    {
        // Reset character
        mario->changeState(new IdleState(mario));
        mario->pos = {100, 0}; 
        mario->movement.velocity = {0, 0}; 
        mario->isGrounded = true; 
    }

}