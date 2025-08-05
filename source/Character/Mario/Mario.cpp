#include "Mario.hpp"
#include "Enemy.hpp" // Needed for dynamic_cast to Enemy
#include "EnemyState.hpp"
#include "Structs.hpp"
#include "FireBall.hpp"
#include "ProjectileState.hpp"
#include "Game.hpp"
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
    setFrame(marioStateType::GROW, 57, 63);
    setFrame(marioStateType::UNGROW, 63, 57);
    setFrame(marioStateType::DEAD, 6, 6);
    setFrame(marioStateType::THROWFB, 52, 52);
    setTexture("Mario2D");
    changeState(new IdleState(this));
}
void Mario::changeForm(MarioForm form)
{
    switch (form)
    {
    case SMALL:
        setFrame(marioStateType::IDLE, 0, 0);
        setFrame(marioStateType::WALK, 1, 3);
        setFrame(marioStateType::RUN, 1, 3);
        setFrame(marioStateType::JUMP, 5, 5);
        setFrame(marioStateType::FALL, 5, 5);
        setFrame(marioStateType::SKID, 4, 4);
        setFrame(marioStateType::CROUCH, 0, 0);
        setFrame(marioStateType::GROW, 57, 63);
        setFrame(marioStateType::UNGROW, 0, 0);
        setFrame(marioStateType::DEAD, 6, 6);
        break;

    case BIG:
        setFrame(marioStateType::IDLE, 17, 17);
        setFrame(marioStateType::WALK, 18, 20);
        setFrame(marioStateType::JUMP, 22, 22);
        setFrame(marioStateType::FALL, 22, 22);
        setFrame(marioStateType::SKID, 21, 21);
        setFrame(marioStateType::RUN, 18, 20);
        setFrame(marioStateType::CROUCH, 23, 23);
        setFrame(marioStateType::GROW, 64, 70);
        setFrame(marioStateType::UNGROW, 63, 57);
        break;

    case FIRE:
        setFrame(marioStateType::IDLE, 37, 37);
        setFrame(marioStateType::WALK, 38, 40);
        setFrame(marioStateType::JUMP, 42, 42);
        setFrame(marioStateType::FALL, 42, 42);
        setFrame(marioStateType::SKID, 41, 41);
        setFrame(marioStateType::RUN, 38, 40);
        setFrame(marioStateType::CROUCH, 43, 43);
        setFrame(marioStateType::GROW, 37, 37);
        setFrame(marioStateType::UNGROW, 70, 64);
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
            this->changeState(new UnGrowState(this));
        }
    }
}
void Mario::update()
{
    Character::update();
    sinceLastThrow += GetFrameTime();
    if (form == FIRE && IsKeyPressed(KEY_Z) && sinceLastThrow > throwPerSecond)
    {
        changeState(new ThrowFBState(this));
    }
}
void Mario::throwFireBall()
{
    FireBall *projectile = new FireBall();
    projectile->setDirection(this->getDirection());
    Vector2 pos = this->getPositionAdapter().toPixels();
    pos.x += this->getDirection() * getSize().x;
    projectile->setPosition(pos);
    cout << "FireBall before: " << projectile << "\n";
    
    Game::addGameObject(projectile);
}

void Mario::createBody(b2World *world)
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
    float radius = halfWidth * 0.75f;
    float torsoHeight = halfHeight - radius;

    float legOffsetY = halfHeight - radius;

    b2PolygonShape torsoShape;
    b2Vec2 torsoOffset(0.0f, 0);
    torsoShape.SetAsBox(halfWidth * 0.6f, torsoHeight, torsoOffset, 0.0f);

    b2FixtureDef torsoFixture;
    torsoFixture.shape = &torsoShape;

    torsoFixture.density = 1.0f;
    torsoFixture.friction = 0.2f;
    torsoFixture.filter.categoryBits = CATEGORY_CHARACTER_MAIN;
    torsoFixture.filter.maskBits = CATEGORY_SOLID | CATEGORY_NOTSOLID | CATEGORY_CHARACTER_SENSOR | CATEGORY_CHARACTER_MAIN;

    torsoFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::NONE);
    body->CreateFixture(&torsoFixture);

    // 2.a Legs (circle) - real collision feet // 2.b Pelvis(rectangle) - to prevent getting stuck on blocks

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
    footSensor.m_radius = radius - 0.5f / PPM;
    footSensor.m_p.Set(0, legOffsetY + 1 / PPM); // Slightly below legs

    b2FixtureDef footFixture;
    footFixture.shape = &footSensor;
    footFixture.isSensor = true;
    footFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::BOTTOM);
    footFixture.filter.categoryBits = CATEGORY_CHARACTER_SENSOR;
    footFixture.filter.maskBits = CATEGORY_SOLID | CATEGORY_CHARACTER_SENSOR;
    body->CreateFixture(&footFixture);

    // 4. Head sensor
    if (form != SMALL)
    {

        // b2PolygonShape headShape;
        float headHeight = 2 / PPM;
        // headShape.SetAsBox(halfWidth * 0.75f, headHeight, b2Vec2(0, -torsoHeight + headHeight), 0);

        // b2FixtureDef headFixture;
        // headFixture.shape = &headShape;
        // headFixture.density = 1.0f;
        // headFixture.friction = 0.2f;
        // headFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::NONE);
        // headFixture.filter.categoryBits = CATEGORY_CHARACTER_MAIN;
        // headFixture.filter.maskBits = CATEGORY_SOLID | CATEGORY_NOTSOLID | CATEGORY_CHARACTER_SENSOR | CATEGORY_CHARACTER_MAIN;
        // body->CreateFixture(&headFixture);

        b2PolygonShape head1Shape;
        head1Shape.SetAsBox(halfWidth * 0.6f, headHeight, b2Vec2(0, -torsoHeight), 0);

        b2FixtureDef head1Fixture;
        head1Fixture.shape = &head1Shape;
        head1Fixture.isSensor = true;
        head1Fixture.userData.pointer = static_cast<uintptr_t>(CollisionType::TOP);
        head1Fixture.filter.categoryBits = CATEGORY_CHARACTER_SENSOR;
        head1Fixture.filter.maskBits = CATEGORY_SOLID | CATEGORY_CHARACTER_SENSOR;
        body->CreateFixture(&head1Fixture);
    }
    else
    {
        // b2PolygonShape headShape;
        float headHeight = 2 / PPM;
        // headShape.SetAsBox(halfWidth * 0.75f, headHeight, b2Vec2(0, -halfHeight + 2 * headHeight), 0);

        // b2FixtureDef headFixture;
        // headFixture.shape = &headShape;
        // headFixture.density = 1.0f;
        // headFixture.friction = 0.2f;
        // headFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::NONE);
        // headFixture.filter.categoryBits = CATEGORY_CHARACTER_MAIN;
        // headFixture.filter.maskBits = CATEGORY_SOLID | CATEGORY_NOTSOLID | CATEGORY_CHARACTER_SENSOR | CATEGORY_CHARACTER_MAIN;
        // body->CreateFixture(&headFixture);

        b2PolygonShape head1Shape;
        head1Shape.SetAsBox(halfWidth * 0.6f, headHeight, b2Vec2(0, -halfHeight + headHeight), 0);

        b2FixtureDef head1Fixture;
        head1Fixture.shape = &head1Shape;
        head1Fixture.isSensor = true;
        head1Fixture.userData.pointer = static_cast<uintptr_t>(CollisionType::TOP);
        head1Fixture.filter.categoryBits = CATEGORY_CHARACTER_SENSOR;
        head1Fixture.filter.maskBits = CATEGORY_SOLID | CATEGORY_CHARACTER_SENSOR;
        body->CreateFixture(&head1Fixture);
    }

    // 5. Left wall sensor
    b2PolygonShape leftWallShape;
    leftWallShape.SetAsBox(2.0f / PPM, size.y() * 0.2f, b2Vec2(-halfWidth / 2, 0), 0);

    b2FixtureDef leftWallFixture;
    leftWallFixture.shape = &leftWallShape;
    leftWallFixture.isSensor = true;
    leftWallFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::LEFTSIDE);
    leftWallFixture.filter.categoryBits = CATEGORY_CHARACTER_SENSOR;
    leftWallFixture.filter.maskBits = CATEGORY_SOLID | CATEGORY_CHARACTER_SENSOR;
    body->CreateFixture(&leftWallFixture);

    // 6. Right wall sensor
    b2PolygonShape rightWallShape;
    rightWallShape.SetAsBox(2.0f / PPM, size.y() * 0.2f, b2Vec2(halfWidth / 2, 0), 0);

    b2FixtureDef rightWallFixture;
    rightWallFixture.shape = &rightWallShape;
    rightWallFixture.isSensor = true;
    rightWallFixture.filter.categoryBits = CATEGORY_CHARACTER_SENSOR;
    rightWallFixture.filter.maskBits = CATEGORY_SOLID | CATEGORY_CHARACTER_SENSOR;
    rightWallFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::RIGHTSIDE);
    body->CreateFixture(&rightWallFixture);

    body->SetBullet(true);
}