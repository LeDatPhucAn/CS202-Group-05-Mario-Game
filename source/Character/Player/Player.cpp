#include "Player.hpp"
#include "MovingObject.hpp" // Needed for dynamic_cast to Enemy
#include "MovingObjectState.hpp"
#include "Structs.hpp"
#include "Game.hpp"
#include "SoundController.hpp"
#include "FireBall.hpp"
Player::Player()
    : Character()
{
    changeForm(SMALL);
}
void Player::EatStar()
{
    starTimer = 12.0f; // star lasts 12 seconds
    starMode = true;
    SoundController::getInstance().playTemporarySceneMusic(sceneType::STAR);
}
void Player::hitByEnemy()
{
    if (isInvincible || starMode)
        return;
    if (form == SMALL)
    {
        changeState(new DeadState(this));
    }
    else
    {
        turnInvincible();
        changeState(new UnGrowState(this));
    }
}
void Player::jumpFromEnemy()
{
    if (starMode)
        return;
    b2Vec2 vel = body->GetLinearVelocity();
    body->SetLinearVelocity({vel.x, 0});
    float mass = body->GetMass();
    b2Vec2 impulse(0, mass * jumpVel / 1.4f);
    body->ApplyLinearImpulseToCenter(impulse, true);
}
void Player::changeForm(PlayerForm _form)
{
    form = _form;
    switch (form)
    {
    case SMALL:
        setFrame(playerStateType::IDLE, 0, 0);
        setFrame(playerStateType::WALK, 1, 3);
        setFrame(playerStateType::RUN, 1, 3);
        setFrame(playerStateType::JUMP, 5, 5);
        setFrame(playerStateType::FALL, 5, 5);
        setFrame(playerStateType::SKID, 4, 4);
        setFrame(playerStateType::CROUCH, 0, 0);
        setFrame(playerStateType::GROW, 57, 63);
        setFrame(playerStateType::UNGROW, 0, 0);
        setFrame(playerStateType::DEAD, 6, 6);
        setFrame(playerStateType::THROWFB, 52, 52);
        break;

    case BIG:
        setFrame(playerStateType::IDLE, 17, 17);
        setFrame(playerStateType::WALK, 18, 20);
        setFrame(playerStateType::JUMP, 22, 22);
        setFrame(playerStateType::FALL, 22, 22);
        setFrame(playerStateType::SKID, 21, 21);
        setFrame(playerStateType::RUN, 18, 20);
        setFrame(playerStateType::CROUCH, 23, 23);
        setFrame(playerStateType::GROW, 64, 70);
        setFrame(playerStateType::UNGROW, 63, 57);
        break;

    case FIRE:
        setFrame(playerStateType::IDLE, 37, 37);
        setFrame(playerStateType::WALK, 38, 40);
        setFrame(playerStateType::JUMP, 42, 42);
        setFrame(playerStateType::FALL, 42, 42);
        setFrame(playerStateType::SKID, 41, 41);
        setFrame(playerStateType::RUN, 38, 40);
        setFrame(playerStateType::CROUCH, 43, 43);
        setFrame(playerStateType::GROW, 37, 37);
        setFrame(playerStateType::UNGROW, 70, 64);
        break;

    default:
        break;
    }
}

void Player::updateCollision(GameObject *other, int type)
{
    if (dynamic_cast<DeadState *>(this->currentState))
    {
        return;
    }
    Character::updateCollision(other, type);
    MovingObject *enemy = dynamic_cast<MovingObject *>(other);
    if (enemy && starMode)
    {
        enemy->changeState(new MovingObjectDeadState(enemy));
        return;
    }
}

void Player::update()
{
    Character::update();
    float deltaTime = GetFrameTime();

    // star
    if (starTimer > 0)
    {
        starTimer -= deltaTime;
        starColorTime += deltaTime * 10; // speed of flashing
    }
    else
    {
        starMode = false;
    }

    if (isInvincible)
    {
        beenInvincibleFor += deltaTime;
        if (beenInvincibleFor >= invincibleTime)
        {
            isInvincible = false;
            beenInvincibleFor = 0.0f;
        }
    }
    // Dead when falling below a certain height
    Vector2 marioPos = this->getPosition();
    float deathY = 500.0f;

    if (marioPos.y > deathY && !dynamic_cast<DeadState *>(this->currentState))
    {
        this->changeState(new DeadState(this));
        return;
    }

    sinceLastThrow += deltaTime;
    if (form == FIRE && IsKeyPressed(getKey(Action::BUTTON_THROW_FIREBALL)) && sinceLastThrow > 0.25f)
    {
        sinceLastThrow = 0;
        throwFireBall();
    }

    if (IsKeyPressed(KEY_X))
    {
        EatStar();
    }
}

void Player::DrawLGBT(Texture2D texture, Rectangle srcRect, Vector2 position)
{
    Color tint = WHITE;
    if (starTimer > 0)
    {
        // Simple rainbow cycling
        float r = (sinf(starColorTime) + 1) * 127;
        float g = (sinf(starColorTime + 2.094f) + 1) * 127; // offset by 120 degrees
        float b = (sinf(starColorTime + 4.188f) + 1) * 127;
        tint = Color{(unsigned char)r, (unsigned char)g, (unsigned char)b, 255};
    }

    DrawTexturePro(
        texture,                                                 // sprite sheet
        srcRect,                                                 // which part of the sheet
        {position.x, position.y, srcRect.width, srcRect.height}, // destination
        {0, 0},                                                  // origin
        0,                                                       // rotation
        tint                                                     // tint color
    );
}
void Player::throwFireBall()
{
    SoundController::getInstance().playPlayerStateSFX(playerStateType::THROWFB);
    throwingFireBall = true;
    FireBall *fireball = new FireBall();

    if (direction == LEFT)
    {
        fireball->setDirection(RIGHT);
    }
    else
        fireball->setDirection(LEFT);
    // fireball->setDirection(this->getDirection());
    Vector2 pos = this->getPositionAdapter().toPixels();
    pos.x += -fireball->getDirection() * getSize().x;
    fireball->setPosition(pos);
    Game::addGameObject(fireball);
}
void Player::display()
{
    // global powerups

    if (throwingFireBall)
    {
        Rectangle frameRec = sprite.frameRecs[52];
        frameRec.width = direction * fabs(frameRec.width);
        if (starMode)
        {
            DrawLGBT(sprite.texture, frameRec, getCenter());
        }
        else
            DrawTextureRec(sprite.texture, frameRec, getCenter(), WHITE);
        currentDelayTime++;
        if (currentDelayTime == throwDelay)
        {
            throwingFireBall = false;
            currentDelayTime = 0;
        }
    }
    else if (starMode)
    {
        DrawLGBT(sprite.texture, currentState->frameRec, getCenter());
    }
    else if (isInvincible)
    {
        invincibleDrawTimer += GetFrameTime();
        if (invincibleDrawTimer >= invincibleDrawRate)
        {
            invincibleDrawTimer = 0.0f;
            Character::display();
        }
    }
    else
    {
        Character::display();
    }
}

void Player::reset()
{
    // Reset mario
    if (body)
    {
        Game::world->DestroyBody(body);
        attachBody(nullptr);
    }
    isInvincible = false;
    starMode = false;
    throwingFireBall = false;
    // Reset Player's position and recreate physics body
    setPosition({200, 50});
    changeForm(SMALL);
    createBody(Game::world); // Recreate the physics body
    changeState(new IdleState(this));
}
void Player::createBody(b2World *world)
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

    // 2.a Legs (circle) - real collision feet

    b2CircleShape legsShape;
    legsShape.m_radius = radius;
    legsShape.m_p.Set(0, legOffsetY); // Bottom center of body
    b2FixtureDef legsFixture;
    legsFixture.shape = &legsShape;
    legsFixture.density = 1.0f;
    legsFixture.friction = 0.1f; // May adjust if sliding on slopes
    legsFixture.filter.categoryBits = CATEGORY_CHARACTER_MAIN;
    legsFixture.filter.maskBits = CATEGORY_SOLID | CATEGORY_NOTSOLID | CATEGORY_CHARACTER_SENSOR | CATEGORY_CHARACTER_MAIN;
    legsFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::NONE);
    body->CreateFixture(&legsFixture);

    // 3. Foot sensor (same size as legs, but offset slightly lower)
    b2CircleShape footSensor;
    footSensor.m_radius = radius - 0.5f / PPM;
    footSensor.m_p.Set(0, legOffsetY + 2 / PPM); // Slightly below legs

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