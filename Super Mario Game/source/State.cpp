#include "../header/State.hpp"
#include "../header/Character.hpp" 

// ---------- Base State ----------
State::State() : numFrames(0), delay(0), character(nullptr), frameRec({ 0 }) {}

State::State(int _numFrames, Character* _character, int _delay)
    : numFrames(_numFrames), delay(_delay), character(_character) {
    frameRec = { 0, 0, (float)character->frameWidth, (float)character->frameHeight};
}

void State::animate() {
    delayCounter++;
    if (delayCounter > delay) {
        delayCounter = 0;
        frameIndex = (frameIndex + 1) % numFrames;
        frameRec.x = character->frameHeight * frameIndex;
    }
}

// ---------- IdleState ----------
IdleState::IdleState() : State() {}

IdleState::IdleState(int _numFrames, Character* _character, int _delay)
    : State(_numFrames, _character, _delay) {
}
void IdleState::handleInput() {
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT)) {
        character->changeState(new WalkState(character->stateFrames[WALK], character));
    }
    else {
        character->velocity.x = 0;
    }
}

void IdleState::updateState() {
    handleInput();
    animate();
    character->pos = Vector2Add(character->pos, character->velocity);
}

void IdleState::displayState() {
    DrawTextureRec(character->sprite, frameRec, character->pos, WHITE);
}

// ---------- WalkState ----------
WalkState::WalkState() : State() {}

WalkState::WalkState(int _numFrames, Character* _character, int _delay)
    : State(_numFrames, _character, _delay) {
}

void WalkState::handleInput() {
    if (IsKeyDown(KEY_RIGHT)) {
        character->velocity.x = character->speed;
    }
    else if (IsKeyDown(KEY_LEFT)) {
        character->velocity.x = -character->speed;
    }
    else {
        character->changeState(new IdleState(character->stateFrames[IDLE], character));
    }
}

void WalkState::updateState() {
    handleInput();
    animate();
    character->pos = Vector2Add(character->pos, character->velocity);
}

void WalkState::displayState() {
    DrawTextureRec(character->sprite, frameRec, character->pos, WHITE);
}

