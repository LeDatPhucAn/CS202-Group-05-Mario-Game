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
    frameRec.width = character->direction * abs(frameRec.width); // change character direction
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
    
    // apply gravity
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
    // ask from map
    if (character->movement.pos.y >= GroundPosY -frameRec.height) {
        character->movement.pos.y = GroundPosY - frameRec.height;
        character->movement.velocity.y = 0;
        character->isGrounded = true;
    }
}

void State::displayState() {
    DrawTextureRec(character->sprite.texture, frameRec, character->movement.pos, WHITE);
}

