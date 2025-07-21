#include "Character.hpp"
#include "GameObject.hpp"
#include "ContactListener.hpp"
#include "PiranhaPlant.hpp"
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
            if (type == TOP)
            {
            }
            else if (type == BOTTOM)
            {
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
    StartEndFrame se = sprite.StartEndFrames[IDLE];
    Rectangle frameRec = sprite.frameRecs[se.start];
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
    b2Vec2 torsoOffset(0.0f, -halfHeight + torsoHalfHeight);
    torsoShape.SetAsBox(halfWidth * 0.75f, torsoHalfHeight, torsoOffset, 0.0f);

    b2FixtureDef torsoFixture;
    torsoFixture.shape = &torsoShape;

    torsoFixture.density = 1.0f;
    torsoFixture.friction = 0.2f;
    torsoFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::NONE);
    body->CreateFixture(&torsoFixture);

    // 2. Legs (circle) - real collision feet

    float radius = halfWidth * 0.75f;
    b2CircleShape legsShape;
    legsShape.m_radius = radius;
    legsShape.m_p.Set(0, radius / 2); // Bottom center of body

    b2FixtureDef legsFixture;
    legsFixture.shape = &legsShape;
    legsFixture.density = 1.0f;
    legsFixture.friction = 0.2f; // May adjust if sliding on slopes
    legsFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::NONE);
    body->CreateFixture(&legsFixture);

    if (dynamic_cast<PiranhaPlant *>(this)){
        b2Fixture* fixture = body->GetFixtureList();
        while (fixture){
            fixture->SetSensor(true);
            fixture = fixture->GetNext();
        }
    }
    // 3. Foot sensor (same size as legs, but offset slightly lower)
    b2CircleShape footSensor;
    footSensor.m_radius = radius;
    footSensor.m_p.Set(0, (radius + (2.0f / PPM)) / 2); // Slightly below legs

    b2FixtureDef footFixture;
    footFixture.shape = &footSensor;
    footFixture.isSensor = true;
    footFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::BOTTOM);
    body->CreateFixture(&footFixture);

    // 4. Head sensor
    b2PolygonShape headShape;
    headShape.SetAsBox(halfWidth * 0.6f, 2.0f / PPM, b2Vec2(0, -halfHeight), 0);

    b2FixtureDef headFixture;
    headFixture.shape = &headShape;
    headFixture.isSensor = true;
    headFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::TOP);
    body->CreateFixture(&headFixture);

    // 5. Left wall sensor
    b2PolygonShape leftWallShape;
    leftWallShape.SetAsBox(2.0f / PPM, size.y() * 0.2f, b2Vec2(-halfWidth / 1.8, 0), 0);

    b2FixtureDef leftWallFixture;
    leftWallFixture.shape = &leftWallShape;
    leftWallFixture.isSensor = true;
    leftWallFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::LEFTSIDE);
    body->CreateFixture(&leftWallFixture);

    // 6. Right wall sensor
    b2PolygonShape rightWallShape;
    rightWallShape.SetAsBox(2.0f / PPM, size.y() * 0.2f, b2Vec2(halfWidth / 1.8, 0), 0);

    b2FixtureDef rightWallFixture;
    rightWallFixture.shape = &rightWallShape;
    rightWallFixture.isSensor = true;
    rightWallFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::RIGHTSIDE);
    body->CreateFixture(&rightWallFixture);
}
