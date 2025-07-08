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
    // cout << "Before " << stateTypeToString(type) << " Physics: " << character->pos.x << " " << character->pos.y << " "
    //      << frameRec.width << " " << frameRec.height << " " << boolalpha << character->isGrounded << "\n";

    float deltaTime = GetFrameTime();
    // Movement
    applyPhysics(deltaTime);

    // collision
    Game::collisionManager.ManageCollision(character, deltaTime);

    // change character direction accordingly
    frameRec.width = character->direction * abs(frameRec.width);

    // Change State when pressing
    handleInput();
    // cout << "After " << stateTypeToString(type) << " handleInput: " << bounds.x << " " << bounds.y << " "
    //      << bounds.width << " " << bounds.height << " " << boolalpha << character->isGrounded << "\n";
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        character->pos = Program::mouseWorldPos;
    }
}

void State::applyPhysics(float deltaTime)
{
    // apply gravity
    if (!character->isGrounded)
    {
        if (character->movement.velocity.y < 0)
        { // Going up
            if (IsKeyDown(KEY_UP) && character->pos.y > maxHeight)
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
    // Apply Collision and physics

    // cout << "pos: " << character->pos.x << " "
    //      << character->pos.y << "; velocity: "
    //      << character->movement.velocity.x << " "
    //      << character->movement.velocity.y << "; "
    //      << "pos change: " << character->movement.velocity.x * deltaTime << " "
    //      << character->movement.velocity.y * deltaTime << "; "
    // cout << stateTypeToString(type) << "\n";
    //      << ((character->direction == RIGHT) ? " RIGHT " : " LEFT ")
    //      << ((character->isGrounded) ? "Grounded\n" : "Not Grounded\n");
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
        fabs(frameRec.width),
        frameRec.height};
}

Rectangle State::Feet() const
{
    return Rectangle{
        character->pos.x + 2,
        character->pos.y + frameRec.height - 2, // Feet is at the bottom of the sprite
        fabs(frameRec.width) - 4,
        5.0f}; // Height of 2 pixel for feet, 3f for collision padding
}
Rectangle State::Head() const
{
    return Rectangle{
        character->pos.x + 2,
        character->pos.y, // Top of the sprite
        fabs(frameRec.width) - 4,
        2 // Height of 2 pixels
    };
}
Rectangle State::LeftSide() const
{
    return Rectangle{
        character->pos.x,         // Left edge
        character->pos.y + 2,     // Slightly inset from top
        2,                        // Narrow width
        fabs(frameRec.height) - 4 // Full height minus padding
    };
}
Rectangle State::RightSide() const
{
    return Rectangle{
        character->pos.x + fabs(frameRec.width) - 2, // Right edge - 2px
        character->pos.y + 2,                        // Slightly inset from top
        2,                                           // Narrow width
        fabs(frameRec.height) - 4                    // Full height minus padding
    };
}
