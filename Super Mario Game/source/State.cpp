#include "../header/State.hpp"
#include "../header/Character.hpp" 

// ---------- Base State ----------
State::State() : delay(0), character(nullptr), delayCounter(0), frameIndex(0), type(IDLE), numFrames(0) {}

State::State(stateType Type, Character* _character, int _delay)
    : delay(_delay), character(_character), delayCounter(_delay), type(Type), frameIndex(0) {
    StartEndFrame se = character->sprite.StartEndFrames[Type];
    numFrames = abs(se.end - se.start) + 1;
    frameRec = character->sprite.frameRecs[se.start];
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

void State::applyPhysics(float deltaTime) {
    if (!character->isGrounded) {
        if (character->movement.velocity.y < 0) { // Going up
            if (IsKeyDown(KEY_UP) && character->movement.pos.y > GroundPosY - maxHeight) {
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

    cout << character->movement.velocity.x << " " << character->movement.velocity.y << " " << type<<" \n";
    // Ground collision
    if (character->movement.pos.y >= GroundPosY) {
        character->movement.pos.y = GroundPosY;
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
    if (IsKeyPressed(KEY_UP) && character->isGrounded) {
        character->changeState(new JumpState(character));
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

        if (abs(character->movement.velocity.x) < 10.0f) {
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

    if (character->movement.velocity.y >= 0) {
        character->changeState(new FallState(character));
        return;
    }
    else if (IsKeyDown(KEY_RIGHT)) {
        character->direction = RIGHT;
        character->movement.acceleration.x = airAccel;
    }
    else if (IsKeyDown(KEY_LEFT)) {
        character->direction = LEFT;
        character->movement.acceleration.x = -airAccel;
    }
}

// ---------- FallState ----------
FallState::FallState() : State() {}

FallState::FallState(Character* _character, int _delay)
    : State(FALL, _character, _delay) {
}

void FallState::handleInput() {

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
        character->movement.acceleration.x = airAccel;
    }
    else if (IsKeyDown(KEY_LEFT)) {
        character->direction = LEFT;
        character->movement.acceleration.x = -airAccel;
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
        character->movement.acceleration.x = skidDecel;
    }
    else if (IsKeyDown(KEY_LEFT)) {

        character->direction = LEFT;
        character->movement.acceleration.x = -skidDecel;
    }
    else {
        character->movement.acceleration.x = character->direction * (-friction);
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
