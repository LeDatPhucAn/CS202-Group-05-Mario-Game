#include "Mario.hpp"
#include "Enemy.hpp" // Needed for dynamic_cast to Enemy
#include "EnemyState.hpp"

Mario::Mario()
    : Character()
{
    this->sprite.StartEndFrames[IDLE] = {0, 0};
    this->sprite.StartEndFrames[WALK] = {1, 3};
    this->sprite.StartEndFrames[JUMP] = {5, 5};
    this->sprite.StartEndFrames[FALL] = {5, 5};
    this->sprite.StartEndFrames[SKID] = {4, 4};
    this->sprite.StartEndFrames[RUN] = {1, 3};
    this->sprite.StartEndFrames[CROUCH] = {0, 0};
    this->sprite.StartEndFrames[GROW] = {44, 50};
    this->sprite.StartEndFrames[UNGROW] = {50, 44};
    this->sprite.StartEndFrames[DEAD] = {6, 6};
    this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Mario2D"]);
    this->sprite.texture = UI::textureMap["Mario2D"];
}
void Mario::changeForm(MarioForm form)
{
    switch (form)
    {
    case SMALL:
        sprite.StartEndFrames[IDLE] = {0, 0};
        sprite.StartEndFrames[JUMP] = {5, 5};
        sprite.StartEndFrames[FALL] = {5, 5};
        sprite.StartEndFrames[SKID] = {4, 4};
        sprite.StartEndFrames[WALK] = {1, 3};
        sprite.StartEndFrames[RUN] = {1, 3};
        sprite.StartEndFrames[CROUCH] = {0, 0};
        sprite.StartEndFrames[GROW] = {44, 50};
        sprite.StartEndFrames[UNGROW] = {0, 0};
        break;
    case BIG:
        sprite.StartEndFrames[IDLE] = {13, 13};
        sprite.StartEndFrames[WALK] = {16, 14};
        sprite.StartEndFrames[JUMP] = {18, 18};
        sprite.StartEndFrames[FALL] = {18, 18};
        sprite.StartEndFrames[SKID] = {17, 17};
        sprite.StartEndFrames[RUN] = {16, 14};
        sprite.StartEndFrames[CROUCH] = {19, 19};
        sprite.StartEndFrames[GROW] = {28, 28};
        sprite.StartEndFrames[UNGROW] = {50, 44};
        break;
    case FIRE:
        sprite.StartEndFrames[IDLE] = {28, 28};
        sprite.StartEndFrames[WALK] = {31, 29};
        sprite.StartEndFrames[JUMP] = {34, 34};
        sprite.StartEndFrames[FALL] = {34, 34};
        sprite.StartEndFrames[SKID] = {33, 33};
        sprite.StartEndFrames[RUN] = {31, 29};
        sprite.StartEndFrames[CROUCH] = {35, 35};
        sprite.StartEndFrames[GROW] = {28, 28};
        sprite.StartEndFrames[UNGROW] = {13, 13};
        break;
    default:
        break;
    }
}

void Mario::updateCollision(GameObject *other, int type)
{
    if (dynamic_cast<DeadState *>(this->currentState))
    {
        return;
    }
    Character::updateCollision(other, type);
    Enemy *enemy = dynamic_cast<Enemy *>(other);
    if (enemy)
    {   
        static float lastHitTime = 0.0f;
        float currentTime = GetTime(); // Raylib function to get current time
            
        if (currentTime - lastHitTime < 0.2f) // 0.2 second cooldown
        {
            return; // Ignore this hit if too soon after last hit
        }
        lastHitTime = currentTime;
        if (dynamic_cast<DeadState *>(this->currentState) || dynamic_cast<EnemyDeadState *>(enemy->currentState))
        {
            return;
        }
        if (type == BOTTOM)
        {
            float mass = this->body->GetMass();
            b2Vec2 impulse(0, mass * jumpVel / 1.5f);
            this->body->ApplyLinearImpulseToCenter(impulse, true);
        }
        else
        {
            if (dynamic_cast<EnemyIdleState *>(enemy->currentState))
            {
                return;
            }
            else this->changeState(new DeadState(this));
        }
    }
}

void Mario::createBody(b2World *world)
{
    Character::createBody(world);
    // StartEndFrame se = sprite.StartEndFrames[currentState->type];
    // Rectangle frameRec = (se.start <= se.end) ? sprite.frameRecs[se.start + currentState->frameIndex]
    //                                           : sprite.frameRecs[se.start - currentState->frameIndex];

    // setSizeAdapter({frameRec.width, frameRec.height});
    // float posX = pos.toMeters().x;
    // float posY = pos.toMeters().y;
    // float halfWidth = size.getHalfWidth();
    // float halfHeight = size.getHalfHeight();

    // b2BodyDef bodyDef;
    // bodyDef.type = b2_dynamicBody;
    // bodyDef.position.Set(posX, posY);
    // bodyDef.fixedRotation = true;
    // bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
    // body = world->CreateBody(&bodyDef);

    // // 1. Torso fixture (rectangle) - slightly shorter
    // float torsoHeight = halfHeight;
    // float torsoHalfHeight = torsoHeight / 2.0f;

    // b2PolygonShape torsoShape;
    // b2Vec2 torsoOffset(0.0f, -torsoHalfHeight);
    // torsoShape.SetAsBox(halfWidth * 0.75f, torsoHalfHeight, torsoOffset, 0.0f);

    // b2FixtureDef torsoFixture;
    // torsoFixture.shape = &torsoShape;

    // torsoFixture.density = 1.0f;
    // torsoFixture.friction = 0.2f;
    // torsoFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::NONE);
    // body->CreateFixture(&torsoFixture);

    // // 2. Legs (circle) - real collision feet

    // float radius = halfWidth * 0.75f;

    // float legOffsetY = torsoHalfHeight * 0.75 + radius / 2.0f;
    // if (size.toPixels().y < 21)
    //     legOffsetY = radius / 2.0f; // Position below torso

    // b2CircleShape legsShape;
    // legsShape.m_radius = radius;
    // legsShape.m_p.Set(0, legOffsetY); // Bottom center of body
    // b2FixtureDef legsFixture;
    // legsFixture.shape = &legsShape;
    // legsFixture.density = 1.0f;
    // legsFixture.friction = 0.2f; // May adjust if sliding on slopes
    // legsFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::NONE);
    // body->CreateFixture(&legsFixture);

    // // 3. Foot sensor (same size as legs, but offset slightly lower)
    // b2CircleShape footSensor;
    // footSensor.m_radius = radius;
    // footSensor.m_p.Set(0, legOffsetY + 1 / PPM); // Slightly below legs

    // b2FixtureDef footFixture;
    // footFixture.shape = &footSensor;
    // footFixture.isSensor = true;
    // footFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::BOTTOM);
    // body->CreateFixture(&footFixture);

    // // 4. Head sensor
    // b2PolygonShape headShape;
    // headShape.SetAsBox(halfWidth * 0.6f, 2.0f / PPM, b2Vec2(0, -halfHeight), 0);

    // b2FixtureDef headFixture;
    // headFixture.shape = &headShape;
    // headFixture.isSensor = true;
    // headFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::TOP);
    // body->CreateFixture(&headFixture);

    // // 5. Left wall sensor
    // b2PolygonShape leftWallShape;
    // leftWallShape.SetAsBox(2.0f / PPM, size.y() * 0.2f, b2Vec2(-halfWidth, 0), 0);

    // b2FixtureDef leftWallFixture;
    // leftWallFixture.shape = &leftWallShape;
    // leftWallFixture.isSensor = true;
    // leftWallFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::LEFTSIDE);
    // body->CreateFixture(&leftWallFixture);

    // // 6. Right wall sensor
    // b2PolygonShape rightWallShape;
    // rightWallShape.SetAsBox(2.0f / PPM, size.y() * 0.2f, b2Vec2(halfWidth, 0), 0);

    // b2FixtureDef rightWallFixture;
    // rightWallFixture.shape = &rightWallShape;
    // rightWallFixture.isSensor = true;
    // rightWallFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::RIGHTSIDE);
    // body->CreateFixture(&rightWallFixture);
}