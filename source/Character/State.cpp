#include "State.hpp"
#include "Character.hpp"
#include "Game.hpp"
#include "Program.hpp"

string stateTypeToString(stateType state)
{
    switch (state)
    {
    case stateType::IDLE:
        return "IDLE";
    case stateType::WALK:
        return "WALK";
    case stateType::RUN:
        return "RUN";
    case stateType::JUMP:
        return "JUMP";
    case stateType::FALL:
        return "FALL";
    case stateType::SKID:
        return "SKID";
    case stateType::CROUCH:
        return "CROUCH";
    case stateType::GROW:
        return "GROW";
    case stateType::UNGROW:
        return "UNGROW";
    default:
        return "UNKNOWN";
    }
}
// ---------- Base State ----------
State::State() : delay(0), character(nullptr), delayCounter(0), frameIndex(0), type(IDLE), numFrames(0) {}

State::State(stateType Type, Character *_character, int _delay)
    : delay(_delay), character(_character), delayCounter(_delay), type(Type), frameIndex(0)
{
    StartEndFrame se = character->sprite.StartEndFrames[Type];
    numFrames = abs(se.end - se.start) + 1;
    frameRec = character->sprite.frameRecs[se.start];
    frameRec.width = character->direction * abs(frameRec.width);
}

void State::animate()
{
    delayCounter++;
    if (delayCounter > delay)
    {
        delayCounter = 0;
        frameIndex = (frameIndex + 1) % numFrames;
        StartEndFrame se = character->sprite.StartEndFrames[type];
        frameRec = (se.start <= se.end) ? character->sprite.frameRecs[se.start + frameIndex] : character->sprite.frameRecs[se.start - frameIndex];
    }
}

void State::updateState()
{
    // Set frame
    animate();

    float deltaTime = GetFrameTime();
    // Movement
    applyPhysics(deltaTime);

    // change character direction accordingly
    frameRec.width = character->direction * abs(frameRec.width);

    // Change State when pressing
    handleInput();

    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
    {
        character->pos = Program::mouseWorldPos;
    }
    // Rectangle bounds = Bounds();
    // cout << "State: " << stateTypeToString(type) << ", Frame: " << frameIndex
    //      << ", Position: (" << character->pos.x << ", " << character->pos.y << "), Bounds: ("
    //      << bounds.x << ", " << bounds.y << ", " << bounds.width << ", " << bounds.height << ")\n";
    character->isGrounded = false; // Reset grounded state for next frame
}

void State::applyPhysics(float deltaTime)
{
    // apply gravity
    if (!character->isGrounded)
    {
        if (character->movement.velocity.y < 0)
        { // Going up
            if (IsKeyDown(KEY_UP) && character->pos.y > character->maxHeight)
            {
                character->movement.acceleration.y = jumpGravity;
            }
            else
            {
                character->movement.acceleration.y = fallGravity;
            }
        }
        else
        {
            character->movement.acceleration.y = fallGravity; // Going down
        }
    }
    else
    {
        character->movement.acceleration.y = 0;
    }

    character->movement.velocity += character->movement.acceleration * deltaTime;

    if (character->movement.velocity.y > fallSpeedCap)
        character->movement.velocity.y = fallSpeedCap;

    character->pos += character->movement.velocity * deltaTime;
}

void State::displayState()
{
    DrawRectangleLines(
        character->pos.x,
        character->pos.y,
        fabs(frameRec.width),
        frameRec.height,
        {168, 168, 0, 255}); // Draw a transparent rectangle for debugging
    DrawTextureRec(character->sprite.texture, frameRec, character->pos, WHITE);
}

Rectangle State::Bounds() const
{
    return Rectangle{
        character->pos.x,
        character->pos.y,
        16,
        18};
}
Rectangle State::ActualBounds() const
{
    return Rectangle{
        character->pos.x,
        character->pos.y,
        16,
        16};
}
// Rectangle State::ActualBounds() const
// {
//     return Rectangle{
//         character->pos.x,
//         character->pos.y,
//         fabs(frameRec.width),
//         frameRec.height};
// }
// Rectangle State::ActualBounds() const
// {
//     Rectangle bounds = character->sprite.frameRecs[character->sprite.StartEndFrames[IDLE].start];
//     bounds.x = character->pos.x;
//     bounds.y = character->pos.y;
//     bounds.width = fabs(bounds.width);
//     return bounds;
// }
Rectangle State::Feet() const
{
    return Rectangle{
        character->pos.x + 4,
        character->pos.y + frameRec.height - 2, // Feet is at the bottom of the sprite
        fabs(frameRec.width) - 8,
        5.0f}; // Height of 2 pixel for feet, 3f for collision padding
}
Rectangle State::Head() const
{
    return Rectangle{
        character->pos.x + 4,
        character->pos.y, // Top of the sprite
        fabs(frameRec.width) - 8,
        2 // Height of 2 pixels
    };
}
Rectangle State::LeftSide() const
{
    return {
        character->pos.x + 0.5f, // Small inward offset to avoid pixel snag
        character->pos.y + 2,
        2,
        frameRec.height - 4};
}

Rectangle State::RightSide() const
{
    return {
        character->pos.x + fabs(frameRec.width) - 2.5f, // Small inward offset
        character->pos.y + 2,
        2,
        frameRec.height - 4};
}