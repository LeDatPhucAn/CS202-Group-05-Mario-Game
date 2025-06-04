#include "../header/State.hpp"
#include "../header/Character.hpp" 

// ---------- Base State ----------
State::State() : delay(0), character(nullptr) {}

State::State(stateType Type, Character* _character, int _delay) : delay(_delay), character(_character) {
    delayCounter = delay;
    type = Type;
    numFrames = abs(character->sprite.StartEndFrames[Type].y - character->sprite.StartEndFrames[Type].x) + 1;
    frameRec = character->sprite.frameRecs[character->sprite.StartEndFrames[Type].x];
}

void State::animate() {
    delayCounter++;
    if (delayCounter > delay) {
        delayCounter = 0;
        frameIndex++;
        frameIndex %= numFrames;
        int x = character->sprite.StartEndFrames[type].x;
        int y = character->sprite.StartEndFrames[type].y;
        if (x <= y) {
            frameRec = character->sprite.frameRecs[x + frameIndex];
        }
        else {
            frameRec = character->sprite.frameRecs[x - frameIndex];
        }
    }
}
void State::updateState() {
    animate();
    character->movement.velocity.y += gravity;
    character->movement.pos = Vector2Add(character->movement.pos, character->movement.velocity);
    if (character->movement.pos.y >= 200) {
        character->movement.pos.y = 200;
        character->movement.velocity.y = 0;
        character->isGrounded = true;
    }

    handleInput();

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
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT)) {
        character->changeState(new WalkState(character));
    }
    else if (IsKeyDown(KEY_UP) && character->isGrounded) {
        character->changeState(new JumpState(character));
    }
    else {
        if (!character->facingRight) {
            frameRec.width = -abs(frameRec.width);
        }
        character->movement.velocity.x = 0;
    }
}


// ---------- WalkState ----------
WalkState::WalkState() : State() {}

WalkState::WalkState(Character* _character, int _delay)
    : State(WALK, _character, _delay) {

}

void WalkState::handleInput() {
    if (IsKeyDown(KEY_UP) && character->isGrounded) {
        character->changeState(new JumpState(character));
    }
    else if (IsKeyDown(KEY_RIGHT)) {
        character->facingRight = true;
        frameRec.width = abs(frameRec.width);
        character->movement.velocity.x = character->movement.speed;
    }
    else if (IsKeyDown(KEY_LEFT)) {
        character->facingRight = false;
        frameRec.width = -abs(frameRec.width);
        character->movement.velocity.x = -character->movement.speed;
    }
    else {
        character->changeState(new IdleState(character));
    }
    
}


// ---------- JumpState ----------
JumpState::JumpState() : State() {}

JumpState::JumpState(Character* _character, int _delay)
    : State(JUMP,_character, _delay) {
}

void JumpState::handleInput() {
    if (!character->isGrounded) {
    character->changeState(new FallState(character));
    }
    else if (IsKeyDown(KEY_UP) && character->isGrounded) {
        if (!character->facingRight) {
            frameRec.width = -abs(frameRec.width);
        }
        character->isGrounded = false;
        character->movement.velocity.y = -4 * character->movement.speed;
    }

    else {
        character->changeState(new IdleState(character));
    }
}
// ---------- FallState ----------
FallState::FallState() : State() {}

FallState::FallState(Character* _character, int _delay)
    : State(FALL,_character, _delay) {
}


void FallState::handleInput() {
    if (character->isGrounded) {
        character->changeState(new IdleState(character));
        return;
    }

    if (!character->facingRight) {
        frameRec.width = -abs(frameRec.width);
    }

    if (IsKeyDown(KEY_RIGHT)) {
        character->facingRight = true;
        frameRec.width = abs(frameRec.width);
        character->movement.velocity.x = character->movement.speed;
    }
    else if (IsKeyDown(KEY_LEFT)) {
        character->facingRight = false;
        frameRec.width = -abs(frameRec.width);
        character->movement.velocity.x = -character->movement.speed;
    }
    else {
        character->movement.velocity.x = 0;
    }
}



