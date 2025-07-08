#include "MarioState.hpp"
#include "Character.hpp"
// ---------- IdleState ----------

IdleState::IdleState(Character *_character, int _delay)
    : MarioState(IDLE, _character, _delay)
{
}

void IdleState::handleInput()
{
    if (IsKeyPressed(KEY_ONE))
    {
        character->changeState(new GrowState(character));
    }
    else if (IsKeyPressed(KEY_TWO))
    {
        character->changeState(new UnGrowState(character));
    }
    else if (IsKeyPressed(KEY_UP) && character->isGrounded)
    {
        character->changeState(new JumpState(character));
    }
    else if (!character->isGrounded)
    {
        character->changeState(new FallState(character));
    }
    else if (IsKeyDown(KEY_DOWN) && character->form != SMALL)
    {
        character->changeState(new CrouchState(character));
    }
    else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT))
    {
        character->changeState(new WalkState(character));
    }
    else
    {
        character->movement.velocity.x = 0;
        character->movement.acceleration.x = 0;
    }
}

// ---------- WalkState ----------

WalkState::WalkState(Character *_character, int _delay)
    : MarioState(WALK, _character, _delay)
{
}

void WalkState::handleInput()
{
    if (IsKeyPressed(KEY_UP) && character->isGrounded)
    {
        character->changeState(new JumpState(character));
        return;
    }
    else if (!character->isGrounded)
    {
        character->changeState(new FallState(character));
    }
    else if (IsKeyPressed(KEY_ONE))
    {
        character->changeState(new GrowState(character));
        return;
    }
    else if (IsKeyPressed(KEY_TWO))
    {
        character->changeState(new UnGrowState(character));
        return;
    }
    else if (IsKeyDown(KEY_LEFT_CONTROL))
    {
        character->changeState(new RunState(character));
        return;
    }
    else if (IsKeyDown(KEY_DOWN) && character->form != SMALL)
    {
        character->changeState(new CrouchState(character));
        return;
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        if (character->direction == LEFT && abs(character->movement.velocity.x) > 20.0f)
        {
            character->changeState(new SkidState(character));
            return;
        }
        character->direction = RIGHT;
        character->movement.acceleration.x = walkAccel;
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        if (character->direction == RIGHT && abs(character->movement.velocity.x) > 20.0f)
        {
            character->changeState(new SkidState(character));
            return;
        }
        character->direction = LEFT;
        character->movement.acceleration.x = -walkAccel;
    }
    else
    {
        character->movement.acceleration.x = character->direction * (-friction);

        if (abs(character->movement.velocity.x) < 20.0f)
        {
            character->changeState(new IdleState(character));
            return;
        }
    }
    // Clamp walk speed
    if (abs(character->movement.velocity.x) > walkSpeed)
    {
        character->movement.velocity.x = walkSpeed * character->direction;
    }
}

// ---------- JumpState ----------

JumpState::JumpState(Character *_character, int _delay)
    : MarioState(JUMP, _character, _delay)
{
    character->isGrounded = false;
    character->movement.velocity.y = jumpVel;
}

void JumpState::handleInput()
{
    float accel = airAccel;
    if (IsKeyDown(KEY_LEFT_CONTROL))
    {
        accel = walkAccel;
    }
    if (character->isGrounded)
    {
        if (abs(character->movement.velocity.x) < 20.0f)
        {
            character->changeState(new IdleState(character));
        }
        else
        {
            character->changeState(new WalkState(character));
        }
        return;
    }
    if (character->movement.velocity.y > 0)
    {
        character->changeState(new FallState(character));
        return;
    }

    if (IsKeyDown(KEY_RIGHT))
    {
        character->direction = RIGHT;
        character->movement.acceleration.x = accel;
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        character->direction = LEFT;
        character->movement.acceleration.x = -accel;
    }
    else
    {
        character->movement.acceleration.x = character->direction * accel / 8;
    }
}

// ---------- FallState ----------
FallState::FallState(Character *_character, int _delay)
    : MarioState(FALL, _character, _delay)
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
    if (character->isGrounded)
    {
        if (abs(character->movement.velocity.x) < 20)
        {
            character->changeState(new IdleState(character));
            return;
        }
        else
        {
            if (character->movement.velocity.x * character->direction < 0)
            {
                character->direction = (character->direction == LEFT) ? RIGHT : LEFT;
            }
            character->changeState(new WalkState(character));
            return;
        }
    }

    if (IsKeyDown(KEY_RIGHT))
    {
        character->direction = RIGHT;
        character->movement.acceleration.x = accel;
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        character->direction = LEFT;
        character->movement.acceleration.x = -accel;
    }
    else
    {
        character->movement.acceleration.x = character->direction * (-airFriction);

        if (abs(character->movement.velocity.x) < 20.0f)
        {
            character->movement.velocity.x = 0;
            character->movement.acceleration.x = 0;
            return;
        }
    }
    // // Clamp air speed
    // if (abs(character->movement.velocity.x) > airSpeed)
    // {
    //     character->movement.velocity.x = airSpeed * character->direction;
    // }
}

// ---------- SkidState ----------

SkidState::SkidState(Character *_character, int _delay)
    : MarioState(SKID, _character, _delay)
{
}

void SkidState::handleInput()
{
    if (abs(character->movement.velocity.x) < 20)
    {
        character->changeState(new IdleState(character));
        return;
    }
    else if (IsKeyPressed(KEY_UP) && character->isGrounded)
    {
        character->changeState(new JumpState(character));
        return;
    }

    if (IsKeyDown(KEY_RIGHT))
    {
        character->direction = RIGHT;
        character->movement.acceleration.x = (character->movement.velocity.x > 0) ? -skidDecel : skidDecel;
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        character->direction = LEFT;
        character->movement.acceleration.x = (character->movement.velocity.x > 0) ? -skidDecel : skidDecel;
    }
    else
    {

        character->movement.acceleration.x = (character->movement.velocity.x > 0) ? -friction * 2 : friction * 2;
    }
}

// ---------- RunState ----------

RunState::RunState(Character *_character, int _delay)
    : MarioState(RUN, _character, _delay)
{
}

void RunState::handleInput()
{
    if (IsKeyPressed(KEY_UP) && character->isGrounded)
    {
        character->changeState(new JumpState(character));
        return;
    }
    if (!IsKeyDown(KEY_LEFT_CONTROL))
    {
        character->changeState(new WalkState(character));
        return;
    }

    if (!character->isGrounded)
    {
        character->changeState(new FallState(character));
        return;
    }
    if (IsKeyDown(KEY_DOWN) && character->form != SMALL)
    {
        character->changeState(new CrouchState(character));
        return;
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        if (character->direction == LEFT && abs(character->movement.velocity.x) > 20.0f)
        {
            character->changeState(new SkidState(character));
            return;
        }
        character->direction = RIGHT;
        character->movement.acceleration.x = runAccel;
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        if (character->direction == RIGHT && abs(character->movement.velocity.x) > 20.0f)
        {
            character->changeState(new SkidState(character));
            return;
        }
        character->direction = LEFT;
        character->movement.acceleration.x = -runAccel;
    }
    else
    {
        character->movement.acceleration.x = character->direction * (-friction);

        if (abs(character->movement.velocity.x) < 20)
        {
            character->changeState(new IdleState(character));
            return;
        }
    }

    // Clamp run speed
    if (abs(character->movement.velocity.x) > runSpeed)
    {
        character->movement.velocity.x = runSpeed * character->direction;
    }
}

// ---------- CrouchState ----------
CrouchState::CrouchState(Character *_character, int _delay)
    : MarioState(CROUCH, _character, _delay)
{
}
void CrouchState::handleInput()
{
    character->movement.acceleration.x = character->direction * (-friction);

    if (!IsKeyDown(KEY_DOWN))
    {
        if (abs(character->movement.velocity.x) < 20.0f)
            character->changeState(new IdleState(character));
        else
            character->changeState(new WalkState(character));
        return;
    }
    if (IsKeyPressed(KEY_UP))
    {
        character->changeState(new JumpState(character));
        return;
    }
    if (abs(character->movement.velocity.x) < 20.0f)
    {
        character->movement.velocity.x = 0;
        character->movement.acceleration.x = 0;
        return;
    }
}

// ---------- GrowState ----------

GrowState::GrowState(Character *_character, int _delay)
    : MarioState(GROW, _character, _delay)
{
    if (character->isGrounded)
        character->pos.y = character->groundPosY - frameRec.height;
}

void GrowState::handleInput()
{
    if (character->form == FIRE)
    {
        character->changeState(new IdleState(character));
        return;
    }
    StartEndFrame se = character->sprite.StartEndFrames[type];

    cout << frameIndex << " " << frameRec.height << boolalpha << character->isGrounded << "\n";
    if (se.start + frameIndex == se.end)
    {
        character->form = static_cast<MarioForm>((character->form + 1) % FORM_COUNT);
        character->changeForm(character->form);
        if (character->isGrounded)
        {
            character->pos.y = character->groundPosY - frameRec.height;
            character->changeState(new IdleState(character));
        }
        else
        {
            character->changeState(new FallState(character));
        }
        return;
    }
}
UnGrowState::UnGrowState(Character *_character, int _delay)
    : MarioState(UNGROW, _character, _delay)
{
    if (character->isGrounded)
        character->pos.y = character->groundPosY - frameRec.height;
}

void UnGrowState::handleInput()
{
    StartEndFrame se = character->sprite.StartEndFrames[type];
    if (character->form == SMALL)
    {
        character->changeState(new IdleState(character));
        return;
    }
    if (character->isGrounded)
        character->pos.y = character->groundPosY - frameRec.height;

    if (se.start - frameIndex == se.end)
    {
        character->form = static_cast<MarioForm>((character->form - 1 + FORM_COUNT) % FORM_COUNT);
        character->changeForm(character->form);
        if (character->isGrounded)
        {
            character->pos.y = character->groundPosY - frameRec.height;
            character->changeState(new IdleState(character));
        }
        else
        {
            character->changeState(new FallState(character));
        }
        return;
    }
}
