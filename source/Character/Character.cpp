#include "Character.hpp"
#include "GameObject.hpp"
#include "ContactListener.hpp"
#include "PiranhaPlant.hpp"
#include "Structs.hpp"

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
            switch (type)
            {
            case TOP:
                // Handle top collision with solid block
                break;
            case BOTTOM:
                // Handle bottom collision with solid block
                break;
            case LEFTSIDE:
                // Handle left side collision with solid block
                break;
            case RIGHTSIDE:
                break;
            }
        }
        else if (!block->isSolid)
        {
            switch (type)
            {
            case TOP:
                // Handle top collision with non-solid block
                break;
            case BOTTOM:
                // Handle bottom collision with non-solid block
                break;
            case LEFTSIDE:
                // Handle left side collision with non-solid block
                break;
            case RIGHTSIDE:
                // Handle right side collision with non-solid block
                break;
            }
        }
    }
}

void Character::createBody(b2World *world)
{
    StartEndFrame se = sprite.StartEndFrames[currentState->type];
    Rectangle frameRec = (se.start <= se.end) ? sprite.frameRecs[se.start + currentState->frameIndex]
                                              : sprite.frameRecs[se.start - currentState->frameIndex];

    setSizeAdapter({frameRec.width, frameRec.height});
    float posX = pos.toMeters().x;
    float posY = pos.toMeters().y;
    float halfWidth = size.getHalfWidth();
    float halfHeight = size.getHalfHeight();

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(posX, posY);
    bodyDef.fixedRotation = true;
    bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
    body = world->CreateBody(&bodyDef);

    // 1. Torso fixture (rectangle) - slightly shorter
    float torsoHeight = halfHeight;
    float torsoHalfHeight = torsoHeight / 2.0f;

    b2PolygonShape torsoShape;
    b2Vec2 torsoOffset(0.0f, -torsoHalfHeight);
    torsoShape.SetAsBox(halfWidth * 0.75f, torsoHalfHeight, torsoOffset, 0.0f);

    b2FixtureDef torsoFixture;
    torsoFixture.shape = &torsoShape;

    torsoFixture.density = 1.0f;
    torsoFixture.friction = 0.2f;
    torsoFixture.filter.categoryBits = CATEGORY_CHARACTER_MAIN;
    torsoFixture.filter.maskBits = CATEGORY_SOLID | CATEGORY_NOTSOLID | CATEGORY_CHARACTER_SENSOR | CATEGORY_CHARACTER_MAIN;

    torsoFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::NONE);
    body->CreateFixture(&torsoFixture);

    // 2. Legs (circle) - real collision feet

    float radius = halfWidth * 0.75f;

    float legOffsetY = torsoHalfHeight * 0.75 + radius / 2.0f;
    if (size.toPixels().y < 21)
        legOffsetY = radius / 2.0f; // Position below torso

    b2CircleShape legsShape;
    legsShape.m_radius = radius;
    legsShape.m_p.Set(0, legOffsetY); // Bottom center of body
    b2FixtureDef legsFixture;
    legsFixture.shape = &legsShape;
    legsFixture.density = 1.0f;
    legsFixture.friction = 0.2f; // May adjust if sliding on slopes
    legsFixture.filter.categoryBits = CATEGORY_CHARACTER_MAIN;
    legsFixture.filter.maskBits = CATEGORY_SOLID | CATEGORY_NOTSOLID | CATEGORY_CHARACTER_SENSOR | CATEGORY_CHARACTER_MAIN;
    legsFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::NONE);
    body->CreateFixture(&legsFixture);

    if (dynamic_cast<PiranhaPlant *>(this))
    {
        b2Fixture *fixture = body->GetFixtureList();
        while (fixture)
        {
            fixture->SetSensor(true);
            fixture = fixture->GetNext();
        }
    }
    // 3. Foot sensor (same size as legs, but offset slightly lower)
    b2CircleShape footSensor;
    footSensor.m_radius = radius;
    footSensor.m_p.Set(0, legOffsetY + 0.5f / PPM); // Slightly below legs

    b2FixtureDef footFixture;
    footFixture.shape = &footSensor;
    footFixture.isSensor = true;
    footFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::BOTTOM);
    footFixture.filter.categoryBits = CATEGORY_CHARACTER_SENSOR;
    footFixture.filter.maskBits = CATEGORY_SOLID | CATEGORY_CHARACTER_SENSOR;
    body->CreateFixture(&footFixture);

    // 4. Head sensor
    b2PolygonShape headShape;
    headShape.SetAsBox(halfWidth * 0.6f, 2.0f / PPM, b2Vec2(0, -halfHeight), 0);

    b2FixtureDef headFixture;
    headFixture.shape = &headShape;
    headFixture.isSensor = true;
    headFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::TOP);
    headFixture.filter.categoryBits = CATEGORY_CHARACTER_SENSOR;
    headFixture.filter.maskBits = CATEGORY_SOLID | CATEGORY_CHARACTER_SENSOR;
    body->CreateFixture(&headFixture);

    // 5. Left wall sensor
    b2PolygonShape leftWallShape;
    leftWallShape.SetAsBox(2.0f / PPM, size.y() * 0.2f, b2Vec2(-halfWidth / 1.8, 0), 0);

    b2FixtureDef leftWallFixture;
    leftWallFixture.shape = &leftWallShape;
    leftWallFixture.isSensor = true;
    leftWallFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::LEFTSIDE);
    leftWallFixture.filter.categoryBits = CATEGORY_CHARACTER_SENSOR;
    leftWallFixture.filter.maskBits = CATEGORY_SOLID | CATEGORY_CHARACTER_SENSOR;
    body->CreateFixture(&leftWallFixture);

    // 6. Right wall sensor
    b2PolygonShape rightWallShape;
    rightWallShape.SetAsBox(2.0f / PPM, size.y() * 0.2f, b2Vec2(halfWidth / 1.8, 0), 0);

    b2FixtureDef rightWallFixture;
    rightWallFixture.shape = &rightWallShape;
    rightWallFixture.isSensor = true;
    rightWallFixture.filter.categoryBits = CATEGORY_CHARACTER_SENSOR;
    rightWallFixture.filter.maskBits = CATEGORY_SOLID | CATEGORY_CHARACTER_SENSOR;
    rightWallFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::RIGHTSIDE);
    body->CreateFixture(&rightWallFixture);
}

void Character::toNewBody()
{
    changeBody = true;
}