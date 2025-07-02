#include "State.hpp"
#include "Character.hpp" 

// ---------- Base State ----------
State::State() : delay(0), character(nullptr), delayCounter(0), frameIndex(0), type(IDLE), numFrames(0)  {}

State::State(stateType Type, Character* _character, int _delay)
    : delay(_delay), character(_character), delayCounter(_delay), type(Type), frameIndex(0) {
    StartEndFrame se = character->sprite.StartEndFrames[Type];
    numFrames = abs(se.end - se.start) + 1;
    frameRec = character->sprite.frameRecs[se.start];
    frameRec.width = character->direction * abs(frameRec.width);
    if(character->isGrounded)character->movement.pos.y = GroundPosY - frameRec.height;

}

void State::animate() {
    delayCounter++;
    if (delayCounter > delay) {
        delayCounter = 0;
        frameIndex = (frameIndex + 1) % numFrames;
        StartEndFrame se = character->sprite.StartEndFrames[type];
        frameRec = (se.start <= se.end) ?
            character->sprite.frameRecs[se.start + frameIndex] :
            character->sprite.frameRecs[se.start - frameIndex];
    }
}

void State::updateState() {
    animate();
    applyPhysics(GetFrameTime());
    frameRec.width = character->direction * abs(frameRec.width);
    handleInput();

}
string stateTypeToString(stateType state) {
    switch (state) {
    case stateType::IDLE: return "IDLE";
    case stateType::WALK: return "WALK";
    case stateType::RUN:  return "RUN";
    case stateType::JUMP: return "JUMP";
    case stateType::FALL: return "FALL";
    case stateType::SKID: return "SKID";
    case stateType::CROUCH: return "CROUCH";
    case stateType::GROW: return "GROW";
    case stateType::UNGROW: return "UNGROW";
    default: return "UNKNOWN";
    }
}
void State::applyPhysics(float deltaTime) {
    if (!character->isGrounded) {
        if (character->movement.velocity.y < 0) { // Going up
            if (IsKeyDown(KEY_UP) && character->movement.pos.y > GroundPosY - maxHeight - frameRec.height) {
                character->movement.acceleration.y = jumpGravity;
            }
            else {
                character->movement.acceleration.y = fallGravity;
            }
        }
        else {
            character->movement.acceleration.y = fallGravity; // Going down
        }
    }
    else {
        character->movement.acceleration.y = 0;
    }

    character->movement.velocity += character->movement.acceleration * deltaTime;

    // Clamp terminal fall speed
    if (character->movement.velocity.y > fallSpeedCap)
        character->movement.velocity.y = fallSpeedCap;

    character->movement.pos += character->movement.velocity * deltaTime;

    // cout << character->movement.velocity.x << " "
    //     << character->movement.velocity.y << " "
    //     << stateTypeToString(type)
    //     << ((character->direction == RIGHT) ? " RIGHT\n" : " LEFT\n");
        
    // Ground collision
    if (character->movement.pos.y >= GroundPosY -frameRec.height) {
        character->movement.pos.y = GroundPosY - frameRec.height;
        character->movement.velocity.y = 0;
        character->isGrounded = true;
    }
}

void State::displayState() {
    DrawTextureRec(character->sprite.texture, frameRec, character->movement.pos, WHITE);
}

// ---------- IdleState ----------
IdleState::IdleState() : State() {}

IdleState::IdleState(Character* _character, int _delay)
    : State(IDLE, _character, _delay) {
}

void IdleState::handleInput() {
    if (IsKeyPressed(KEY_ONE)) {
        character->changeState(new GrowState(character));
        return;
    }
    if (IsKeyPressed(KEY_TWO)) {
        character->changeState(new UnGrowState(character));
        return;
    }
    if (IsKeyPressed(KEY_UP) && character->isGrounded) {
        character->changeState(new JumpState(character));
    }
    else if (IsKeyDown(KEY_DOWN) && character->form != SMALL) {
        character->changeState(new CrouchState(character));
	}
    else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT)) {
        character->changeState(new WalkState(character));
    }
    else {
        character->movement.velocity.x = 0;
        character->movement.acceleration.x = 0;
    }
}

// ---------- WalkState ----------
WalkState::WalkState() : State() {}

WalkState::WalkState(Character* _character, int _delay)
    : State(WALK, _character, _delay) {
}

void WalkState::handleInput() {
    if (IsKeyPressed(KEY_UP) && character->isGrounded) {
        character->changeState(new JumpState(character));
        return;
    }
    else if (IsKeyDown(KEY_LEFT_CONTROL)) {
        character->changeState(new RunState(character));
        return;
    }
    else if (IsKeyDown(KEY_DOWN) && character->form != SMALL) {
        character->changeState(new CrouchState(character));
        return;
	}
    if (IsKeyDown(KEY_RIGHT)) {
        if (character->direction == LEFT && abs(character->movement.velocity.x) > 20.0f) {
            character->changeState(new SkidState(character));
            return;
        }
        character->direction = RIGHT;
        character->movement.acceleration.x = walkAccel;
    }
    else if (IsKeyDown(KEY_LEFT)) {
        if (character->direction == RIGHT && abs(character->movement.velocity.x) > 20.0f) {
            character->changeState(new SkidState(character));
            return;
        }
        character->direction = LEFT;
        character->movement.acceleration.x = -walkAccel;
    }
    else {
        character->movement.acceleration.x = character->direction *  (- friction);

        if (abs(character->movement.velocity.x) < 20.0f) {
            character->changeState(new IdleState(character));
            return;
        }
    }
    // Clamp walk speed
    if (abs(character->movement.velocity.x) > walkSpeed) {
        character->movement.velocity.x = walkSpeed * character->direction;
    }
}

// ---------- JumpState ----------
JumpState::JumpState() : State() {}

JumpState::JumpState(Character* _character, int _delay)
    : State(JUMP, _character, _delay) {
    character->isGrounded = false;
    character->movement.velocity.y = jumpVel;
}

void JumpState::handleInput() {
    float accel = airAccel;
    if (IsKeyDown(KEY_LEFT_CONTROL)) {
        accel = walkAccel;
    }
    if (character->movement.velocity.y > 0) {
        character->changeState(new FallState(character));
        return;
    }
    else if (IsKeyDown(KEY_RIGHT)) {
        character->direction = RIGHT;
        character->movement.acceleration.x = accel;
    }
    else if (IsKeyDown(KEY_LEFT)) {
        character->direction = LEFT;
        character->movement.acceleration.x = -accel;
    }
    else {
        character->movement.acceleration.x = character->direction * accel/8;
    }
}

// ---------- FallState ----------
FallState::FallState() : State() {}

FallState::FallState(Character* _character, int _delay)
    : State(FALL, _character, _delay) {
}

void FallState::handleInput() {
    float accel = airAccel;
    if (IsKeyDown(KEY_LEFT_CONTROL)) {
        accel = walkAccel;
    }
    if (character->isGrounded) {
        if (abs(character->movement.velocity.x) < 20) {
            character->changeState(new IdleState(character));
            return;
        }
        else {
            if (character->movement.velocity.x * character->direction < 0) {
                character->direction = (character->direction == LEFT) ? RIGHT : LEFT;
            }
            character->changeState(new WalkState(character));
            return;
        }
    }

    if (IsKeyDown(KEY_RIGHT)) {
        character->direction = RIGHT;
        character->movement.acceleration.x = accel;
    }
    else if (IsKeyDown(KEY_LEFT)) {
        character->direction = LEFT;
        character->movement.acceleration.x = -accel;
    }

}

// ---------- SkidState ----------
SkidState::SkidState() : State() {}

SkidState::SkidState(Character* _character, int _delay)
    : State(SKID, _character, _delay) {
}

void SkidState::handleInput() {
    if (abs(character->movement.velocity.x) < 20) {
        character->changeState(new IdleState(character));
        return;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        character->direction = RIGHT;
        character->movement.acceleration.x = (character->movement.velocity.x > 0) ? -skidDecel : skidDecel;
    }
    else if (IsKeyDown(KEY_LEFT)) {
        character->direction = LEFT;
        character->movement.acceleration.x = (character->movement.velocity.x > 0) ? -skidDecel : skidDecel;
    }
    else {

        character->movement.acceleration.x = (character->movement.velocity.x > 0) ? -friction*2 : friction*2;

    }


}

// ---------- RunState ----------
RunState::RunState() : State() {}

RunState::RunState(Character* _character, int _delay)
    : State(RUN, _character, _delay) {
}

void RunState::handleInput() {
    if (!IsKeyDown(KEY_LEFT_CONTROL)) {
        character->changeState(new WalkState(character));
        return;
    }

    if (IsKeyPressed(KEY_UP) && character->isGrounded) {
        character->changeState(new JumpState(character));
        return;
    }
    if (IsKeyDown(KEY_DOWN) && character->form != SMALL) {
        character->changeState(new CrouchState(character));
        return;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        if (character->direction == LEFT && abs(character->movement.velocity.x) > 20.0f) {
            character->changeState(new SkidState(character));
            return;
        }
        character->direction = RIGHT;
        character->movement.acceleration.x = runAccel;
    }
    else if (IsKeyDown(KEY_LEFT)) {
        if (character->direction == RIGHT && abs(character->movement.velocity.x) > 20.0f) {
            character->changeState(new SkidState(character));
            return;
        }
        character->direction = LEFT;
        character->movement.acceleration.x = -runAccel;
    }
    else {
        character->movement.acceleration.x = character->direction * (- friction);

        if (abs(character->movement.velocity.x) < 20) {
            character->changeState(new IdleState(character));
            return;
        }
    }

    // Clamp run speed
    if (abs(character->movement.velocity.x) > runSpeed) {
        character->movement.velocity.x = runSpeed * character->direction;
    }
}

// ---------- CrouchState ----------
CrouchState::CrouchState() : State() {}
CrouchState::CrouchState(Character* _character, int _delay)
    : State(CROUCH, _character, _delay) {
}
void CrouchState::handleInput() {
    character->movement.acceleration.x = character->direction * (-friction);

    if(!IsKeyDown(KEY_DOWN)) {
        if (abs(character->movement.velocity.x) < 20.0f) character->changeState(new IdleState(character));
		else character->changeState(new WalkState(character));
        return;
	}
    if (IsKeyPressed(KEY_UP)) {
        character->changeState(new JumpState(character));
        return;
    }
    if (abs(character->movement.velocity.x) < 20.0f) {
		character->movement.velocity.x = 0;
		character->movement.acceleration.x = 0;
        return;
    }
}

// ---------- GrowState ----------
GrowState::GrowState() : State() {}

GrowState::GrowState(Character* _character, int _delay)
    : State(GROW, _character, _delay) {
}

void GrowState::handleInput() {
    StartEndFrame se = character->sprite.StartEndFrames[type];
    character->movement.pos.y = GroundPosY - frameRec.height;
    if (character->form == FIRE) {
        character->changeState(new IdleState(character));
        return;
    }
    if (se.start + frameIndex == se.end ) {
        character->form = static_cast<MarioForm>((character->form + 1) % 3);
        character->changeForm(character->form);

        Vector2 newpos = character->movement.pos;
        newpos.y = GroundPosY - frameRec.height;

        character->movement.pos = newpos;

        character->changeState(new IdleState(character));
        return;
    }
}
UnGrowState::UnGrowState() : State() {}

UnGrowState::UnGrowState(Character* _character, int _delay)
    : State(UNGROW, _character, _delay) {
}

void UnGrowState::handleInput() {
    StartEndFrame se = character->sprite.StartEndFrames[type];
    character->movement.pos.y = GroundPosY - frameRec.height;
    if (character->form == SMALL) {
        character->changeState(new IdleState(character));
        return;
    }
    if (se.start - frameIndex == se.end ) {
        character->form = static_cast<MarioForm>((character->form - 1 + FORM_COUNT) % FORM_COUNT);
        character->changeForm(character->form);
        
        Vector2 newpos = character->movement.pos;
        newpos.y = GroundPosY - frameRec.height;

        character->movement.pos = newpos;

        character->changeState(new IdleState(character));
        return;
    }
    
}

