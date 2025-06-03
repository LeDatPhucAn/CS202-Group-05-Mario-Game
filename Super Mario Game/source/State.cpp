#include "../header/State.hpp"
#include "../header/Character.hpp" 

// ---------- Base State ----------
State::State() : delay(0), character(nullptr) {}

State::State(Character* _character, int _delay) : delay(_delay), character(_character) {
    delayCounter = delay;
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
    character->movement.pos = Vector2Add(character->movement.pos, character->movement.velocity);
    handleInput();

}
// ---------- IdleState ----------
IdleState::IdleState() : State() {}

IdleState::IdleState(Character* _character, int _delay)
    : State( _character, _delay) {
    type = IDLE;
    numFrames = abs(character->sprite.StartEndFrames[type].y - character->sprite.StartEndFrames[type].x) + 1;
    frameRec = character->sprite.frameRecs[character->sprite.StartEndFrames[type].x];
}
void IdleState::handleInput() {
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT)) {
        character->changeState(new WalkState(character));
    }
    else {
        character->movement.velocity.x = 0;
    }
}

//void IdleState::updateState() {
//    animate();
//    character->movement.pos = Vector2Add(character->movement.pos, character->movement.velocity);
//    handleInput();
//}

void IdleState::displayState() {
    DrawTextureRec(character->sprite.texture, frameRec, character->movement.pos, WHITE);
}

// ---------- WalkState ----------
WalkState::WalkState() : State() {}

WalkState::WalkState(Character* _character, int _delay)
    : State(_character, _delay) {
    type = WALK;
    numFrames = abs(character->sprite.StartEndFrames[type].y - character->sprite.StartEndFrames[type].x) + 1; 
    frameRec = character->sprite.frameRecs[character->sprite.StartEndFrames[type].x];
}

void WalkState::handleInput() {
    if (IsKeyDown(KEY_RIGHT)) {
        if (frameRec.width < 0) frameRec.width *= -1;
        character->movement.velocity.x = character->movement.speed;
    }
    else if (IsKeyDown(KEY_LEFT)) {
        if (frameRec.width > 0) frameRec.width *= -1;
        character->movement.velocity.x = -character->movement.speed;
    }
    else {
        character->changeState(new IdleState(character));
    }
}

//void WalkState::updateState() {
//    animate();
//    character->movement.pos = Vector2Add(character->movement.pos, character->movement.velocity);
//    handleInput();
//
//}

void WalkState::displayState() {
    DrawTextureRec(character->sprite.texture, frameRec, character->movement.pos, WHITE);
}

