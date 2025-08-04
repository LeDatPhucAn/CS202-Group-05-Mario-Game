#include "State.hpp"
#include "Game.hpp"
#include "Character.hpp"
#include "Program.hpp"
#include "ProjectileState.hpp"
// ---------- Base State ----------
State::State() : delay(0), gameObject(nullptr), delayCounter(0), frameIndex(0), numFrames(0) {}

State::State(int _type, GameObject *_gameObject, int _delay)
    : delay(_delay), gameObject(_gameObject), delayCounter(_delay), type(_type), frameIndex(0)
{
    StartEndFrame se = gameObject->sprite.StartEndFrames[_type];
    numFrames = abs(se.end - se.start) + 1;
    frameRec = gameObject->sprite.frameRecs[se.start];
    frameRec.width = gameObject->direction * abs(frameRec.width);
    gameObject->setSize({fabs(frameRec.width), frameRec.height});
}
State::~State()
{
    numFrames = 0;
    frameIndex = 0;
    delay = 0;
    delayCounter = 0;
    frameRec = {0, 0, 0, 0}; // Assuming Rectangle is a struct with x, y, width, height
    type = 0;
}
void State::animate()
{
    delayCounter++;
    if (delayCounter > delay)
    {
        delayCounter = 0;
        frameIndex = (frameIndex + 1) % numFrames;
        StartEndFrame se = gameObject->sprite.StartEndFrames[type];
        frameRec = (se.start <= se.end) ? gameObject->sprite.frameRecs[se.start + frameIndex] : gameObject->sprite.frameRecs[se.start - frameIndex];
    }
}
std::string marioStateToString(marioStateType state)
{
    switch (state)
    {
    case marioStateType::IDLE:
        return "IDLE";
    case marioStateType::WALK:
        return "WALK";
    case marioStateType::RUN:
        return "RUN";
    case marioStateType::JUMP:
        return "JUMP";
    case marioStateType::FALL:
        return "FALL";
    case marioStateType::SKID:
        return "SKID";
    case marioStateType::CROUCH:
        return "CROUCH";
    case marioStateType::GROW:
        return "GROW";
    case marioStateType::UNGROW:
        return "UNGROW";
    case marioStateType::DEAD:
        return "DEAD";
    case marioStateType::THROWFB:
        return "THROWFB";
    default:
        return "UNKNOWN";
    }
}

void State::updateState()
{

    // if (dynamic_cast<MarioState *>(this))
    // {
    //     cout << "current state is: " << marioStateToString(static_cast<marioStateType>(type)) << "\n";
    // }
    // Update the gameObject's position based on Box2D body
    Vec2Adapter adapter(gameObject->body->GetPosition());
    gameObject->setPositionAdapter(adapter);

    // Check if the character's body needs to be changed
    if (dynamic_cast<Character *>(gameObject))
    {
        Character *character = static_cast<Character *>(gameObject);
        if (character->changeBody)
        {
            // Destroy the old body and create a new one
            Game::world->DestroyBody(character->getBody());
            character->createBody(Game::world);
            character->changeBody = false;
        }
        // apply different gravity when jumping and falling
        if (!character->isGrounded)
        {
            float gravityAccel = (character->body->GetLinearVelocity().y < 0 && (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)))
                                     ? jumpGravity
                                     : addedFallGravity;
            character->body->ApplyForceToCenter({0, character->body->GetMass() * gravityAccel}, true);
        }
    }

    // Set frame
    animate();

    // change character direction accordingly
    frameRec.width = gameObject->direction * abs(frameRec.width);

    // Change State when pressing
    handleInput();
}

void State::displayState()
{
    DrawTextureRec(gameObject->sprite.texture, frameRec, gameObject->getCenter(), WHITE);
}
