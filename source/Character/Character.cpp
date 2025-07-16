#include "Character.hpp"
#include "GameObject.hpp"
#include "ContactListener.hpp"

void Character::changeState(State *newState)
{
    if (currentState)
        delete currentState;
    currentState = newState;
}
void Character::update()
{
    if (currentState)
    {
        currentState->updateState();
    }
}
void Character::display()
{
    if (currentState)
    {
        currentState->displayState();
    }
}

void Character::updateCollision(GameObject *other, int type)
{

    if (Block *block = dynamic_cast<Block *>(other))
    {
        if (block->isSolid)
        {
            if (type == HEAD)
            {
            }
            else if (type == FEET) 
            {
                isGrounded = true;
            }
            else if (type == LEFTSIDE) 
            {
            }
            else if (type == RIGHTSIDE) 
            {
            }
        }
    }
}

void Character::createBody(b2World *world)
{
    StartEndFrame se = sprite.StartEndFrames[WALK];
    Rectangle frameRec = sprite.frameRecs[se.start];
    size.x = frameRec.width;
    size.y = frameRec.height;
    // Convert position and size from pixels to meters
    float posX = pos.x / PPM;
    float posY = pos.y / PPM;
    float halfWidth = size.x * 0.5f / PPM;
    float halfHeight = size.y * 0.5f / PPM;

    // Define the body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(posX, posY);
    bodyDef.fixedRotation = true;
    bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
    body = world->CreateBody(&bodyDef);

    // 1. Main body fixture (physical shape for collisions)
    b2PolygonShape mainShape;
    mainShape.SetAsBox(halfWidth * 0.8f, halfHeight); // 80% width, full height

    b2FixtureDef bodyFixture;
    bodyFixture.shape = &mainShape;
    bodyFixture.density = 1.0f;
    bodyFixture.friction = 0.2f;
    bodyFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::NONE); // Main body doesn't indicate direction
    body->CreateFixture(&bodyFixture);

    // 2. Feet sensor (detects when character is on the ground)
    b2PolygonShape feetShape;
    feetShape.SetAsBox(halfWidth * 0.6f, 2.0f / PPM, b2Vec2(0, halfHeight), 0);

    b2FixtureDef feetFixture;
    feetFixture.shape = &feetShape;
    feetFixture.isSensor = true;
    feetFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::FEET);
    body->CreateFixture(&feetFixture);

    // 3. Head sensor (detects when bumping into blocks from below)
    b2PolygonShape headShape;
    headShape.SetAsBox(halfWidth * 0.6f, 2.0f / PPM, b2Vec2(0, -halfHeight), 0);

    b2FixtureDef headFixture;
    headFixture.shape = &headShape;
    headFixture.isSensor = true;
    headFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::HEAD);
    body->CreateFixture(&headFixture);

    // 4. Left wall sensor
    b2PolygonShape leftWallShape;
    leftWallShape.SetAsBox(2.0f / PPM, size.y * 0.4f / PPM, b2Vec2(-halfWidth, 0), 0);

    b2FixtureDef leftWallFixture;
    leftWallFixture.shape = &leftWallShape;
    leftWallFixture.isSensor = true;
    leftWallFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::LEFTSIDE);
    body->CreateFixture(&leftWallFixture);

    // 5. Right wall sensor
    b2PolygonShape rightWallShape;
    rightWallShape.SetAsBox(2.0f / PPM, size.y * 0.4f / PPM, b2Vec2(halfWidth, 0), 0);

    b2FixtureDef rightWallFixture;
    rightWallFixture.shape = &rightWallShape;
    rightWallFixture.isSensor = true;
    rightWallFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::RIGHTSIDE);
    body->CreateFixture(&rightWallFixture);
}
