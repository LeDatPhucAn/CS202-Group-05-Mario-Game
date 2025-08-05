#include "FireBall.hpp"
#include "ProjectileState.hpp"
#include "Enemy.hpp"
#include "SoundController.hpp"
FireBall::FireBall() : Projectile()
{
    setFrame(projectileStateType::MOVE, 0, 3);
    setFrame(projectileStateType::STOP, 4, 6);
    setTexture("Projectiles2D");

    changeState(new ProjectileMoveState(this));
}

void FireBall::updateCollision(GameObject *other, int type)
{
    Block *block = dynamic_cast<Block *>(other);
    if (block)
    {
        if (block->isSolid)
        {
            switch (type)
            {
            case TOP:
                // Handle top collision with solid block
                break;
            case BOTTOM:
            { // Handle bottom collision with solid
                float mass = getBody()->GetMass();
                b2Vec2 impulse(0, mass * jumpVel / 8);
                getBody()->ApplyLinearImpulseToCenter(impulse, true);
                break;
            }
            case LEFTSIDE:
            { // Handle left side collision with solid block
                changeState(new ProjectileStopState(this));
                break;
            }
            case RIGHTSIDE:
                changeState(new ProjectileStopState(this));
                break;
            }
        }
    }
    Enemy *enemy = dynamic_cast<Enemy *>(other);
    if (enemy)
    {
        enemy->changeState(new EnemyDeadState(enemy));
        changeState(new ProjectileStopState(this));
    }
}
void FireBall::createBody(b2World *world)
{
    float posX = pos.toMeters().x;
    float posY = pos.toMeters().y;
    float halfWidth = size.getHalfWidth();
    float halfHeight = size.getHalfHeight();

    b2BodyDef bodyDef;
    bodyDef.fixedRotation = true;
    bodyDef.type = b2_dynamicBody;

    bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
    bodyDef.position.Set(posX, posY);
    body = world->CreateBody(&bodyDef);

    // Main box collider
    b2PolygonShape boxShape;

    boxShape.SetAsBox(halfWidth, halfHeight);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.filter.categoryBits = CATEGORY_CHARACTER_MAIN;
    fixtureDef.filter.maskBits = CATEGORY_SOLID | CATEGORY_CHARACTER_SENSOR;
    fixtureDef.friction = 0.8f;
    fixtureDef.density = 1.0f;
    // fixtureDef.restitution = 0.75; // Add some bounce effect
    body->CreateFixture(&fixtureDef);

    // 1. Torso fixture (rectangle) - slightly shorter
    float torsoHeight = halfHeight;
    float torsoHalfHeight = torsoHeight / 2.0f;

    // 2. Legs (circle) - real collision feet

    float radius = halfWidth * 0.75f;

    float legOffsetY = torsoHalfHeight * 0.75 + radius / 2.0f;

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
    leftWallShape.SetAsBox(2.0f / PPM, size.y() * 0.5f, b2Vec2(-halfWidth / 1.8, 0), 0);

    b2FixtureDef leftWallFixture;
    leftWallFixture.shape = &leftWallShape;
    leftWallFixture.isSensor = true;
    leftWallFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::LEFTSIDE);
    leftWallFixture.filter.categoryBits = CATEGORY_CHARACTER_SENSOR;
    leftWallFixture.filter.maskBits = CATEGORY_SOLID | CATEGORY_CHARACTER_SENSOR;
    body->CreateFixture(&leftWallFixture);

    // 6. Right wall sensor
    b2PolygonShape rightWallShape;
    rightWallShape.SetAsBox(2.0f / PPM, size.y() * 0.5f, b2Vec2(halfWidth / 1.8, 0), 0);

    b2FixtureDef rightWallFixture;
    rightWallFixture.shape = &rightWallShape;
    rightWallFixture.isSensor = true;
    rightWallFixture.filter.categoryBits = CATEGORY_CHARACTER_SENSOR;
    rightWallFixture.filter.maskBits = CATEGORY_SOLID | CATEGORY_CHARACTER_SENSOR;
    rightWallFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::RIGHTSIDE);
    body->CreateFixture(&rightWallFixture);
}