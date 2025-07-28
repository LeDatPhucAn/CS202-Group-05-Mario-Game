#include "Mario.hpp"
#include "Enemy.hpp" // Needed for dynamic_cast to Enemy
#include "EnemyState.hpp"
#include "Structs.hpp"
Mario::Mario()
    : Character()
{
    setFrame(marioStateType::IDLE, 0, 0);
    setFrame(marioStateType::WALK, 1, 3);
    setFrame(marioStateType::RUN, 1, 3);
    setFrame(marioStateType::JUMP, 5, 5);
    setFrame(marioStateType::FALL, 5, 5);
    setFrame(marioStateType::SKID, 4, 4);
    setFrame(marioStateType::CROUCH, 0, 0);
    setFrame(marioStateType::GROW, 44, 50);
    setFrame(marioStateType::UNGROW, 50, 44);
    setFrame(marioStateType::DEAD, 6, 6);

    setTexture("Mario2D");
}
void Mario::changeForm(MarioForm form)
{
    switch (form)
    {
    case SMALL:
        setFrame(marioStateType::IDLE, 0, 0);
        setFrame(marioStateType::JUMP, 5, 5);
        setFrame(marioStateType::FALL, 5, 5);
        setFrame(marioStateType::SKID, 4, 4);
        setFrame(marioStateType::WALK, 1, 3);
        setFrame(marioStateType::RUN, 1, 3);
        setFrame(marioStateType::CROUCH, 0, 0);
        setFrame(marioStateType::GROW, 44, 50);
        setFrame(marioStateType::UNGROW, 0, 0);
        break;

    case BIG:
        setFrame(marioStateType::IDLE, 13, 13);
        setFrame(marioStateType::WALK, 16, 14);
        setFrame(marioStateType::JUMP, 18, 18);
        setFrame(marioStateType::FALL, 18, 18);
        setFrame(marioStateType::SKID, 17, 17);
        setFrame(marioStateType::RUN, 16, 14);
        setFrame(marioStateType::CROUCH, 19, 19);
        setFrame(marioStateType::GROW, 28, 28);
        setFrame(marioStateType::UNGROW, 50, 44);
        break;

    case FIRE:
        setFrame(marioStateType::IDLE, 28, 28);
        setFrame(marioStateType::WALK, 31, 29);
        setFrame(marioStateType::JUMP, 34, 34);
        setFrame(marioStateType::FALL, 34, 34);
        setFrame(marioStateType::SKID, 33, 33);
        setFrame(marioStateType::RUN, 31, 29);
        setFrame(marioStateType::CROUCH, 35, 35);
        setFrame(marioStateType::GROW, 28, 28);
        setFrame(marioStateType::UNGROW, 13, 13);
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
            this->changeState(new DeadState(this));
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