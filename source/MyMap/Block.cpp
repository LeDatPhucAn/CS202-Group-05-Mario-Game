#include "Block.hpp"
#include <iostream>
#include "Mario.hpp"
using namespace std;

Block::Block(int _gid, Vector2 _pos, Vector2 _size,
             Texture2D _tex, Rectangle _src)
    : GameObject(_pos, _size), gid(_gid), texture(_tex), srcRec(_src), prePos(_pos)
{
}

Block::Block(tson::Object &obj, Vector2 _pos, Vector2 _size,
             Texture2D _tex, Rectangle _src)
    : GameObject(_pos, _size), gid(obj.getGid()), texture(_tex), srcRec(_src), prePos(_pos)
{
    isSolid = obj.get<bool>("isSolid");
    isBreakable = obj.get<bool>("isBreakable");
    isQuestion = obj.get<bool>("isQuestion");
    isInvisible = obj.get<bool>("isInvisible");
    isUsed = obj.get<bool>("isUsed");

    // containsItem trong Tiled lưu dưới dạng int
    int iv = obj.get<int>("containsItem");
    contains = static_cast<Contains>(iv);

    isPipeEntrance = obj.get<bool>("isPipeEntrance");
    isFlagPole = obj.get<bool>("isFlagPole");

    // sprites.texture = _tex;
    // sprites.frameRecs.push_back(_src);
    // sprites.StartEndFrames.push_back({0, 0}); // Chỉ có 1 frame
}

Block::Block(tson::Tile *inforTile, Vector2 _pos, Vector2 _size,
             Texture2D _tex, Rectangle _src, const TSInfo *tsi, Vector2 saved, vector<Rectangle> _fullFrame)
    : GameObject(_pos, _size), gid(inforTile->getGid()), texture(_tex), srcRec(_src), prePos(_pos), StartEnd(saved)
{

    // trong Block::Block(...):
    behavior = shared_ptr<IBlockBehavior>(
        FactoryIBlockBehavior::create(inforTile->getType(), this));
    if (!behavior)
        throw std::runtime_error("Unknown block type: " + inforTile->getType());

    tson::Animation animation = inforTile->getAnimation();

    if (animation.any())
    {
        auto frames = animation.getFrames();

        duration = frames[0].getDuration();
        Rectangle tmpRec = srcRec;
        for (int i = 0; i < frames.size(); ++i)
        {
            sprite.frameRecs.push_back({tmpRec.x + i * (tsi->tileSize.x + tsi->spacing),
                                        tmpRec.y,
                                        tmpRec.width,
                                        tmpRec.height});
        }
        StartEnd.y += frames.size() - 1;
    }
    else
        sprite.frameRecs.push_back(_src);
    sprite.frameRecs = _fullFrame;

    sprite.StartEndFrames[IDLE] = {(int)(StartEnd.x), (int)StartEnd.y};
    sprite.texture = _tex;

    isSolid = inforTile->get<bool>("isSolid");
    isBreakable = inforTile->get<bool>("isBreakable");
    isQuestion = inforTile->get<bool>("isQuestion");
    isInvisible = inforTile->get<bool>("isInvisible");
    isUsed = inforTile->get<bool>("isUsed");

    int iv = inforTile->get<int>("containsItem");
    contains = static_cast<Contains>(iv);

    isPipeEntrance = inforTile->get<bool>("isPipeEntrance");
    isFlagPole = inforTile->get<bool>("isFlagPole");
}

void Block::update()
{
    float dt = GetFrameTime();
    behavior->updateFrame(dt);
    if (currentState)
    {
        currentState->updateState();
    }
}

void Block::display()
{
    // if (isInvisible && !isUsed) return;

    if (currentState)
    {
        currentState->displayState();
    }

    // float dt = GetFrameTime();

    // // Animation Display
    // if (srcRecs.size() == 0)
    //     DrawTextureRec(texture, srcRec, getCenter(), color);
    // else
    // {
    //     aniTimer += GetFrameTime() * 1000; // đổi qua mili giây vì duration ms
    //     if (aniTimer > duration)
    //     {
    //         aniIndex = (aniIndex + 1) % srcRecs.size();
    //         aniTimer = 0;
    //     }
    //     DrawTextureRec(texture, srcRecs[aniIndex], getCenter(), color);
    //     // cout << aniTimer << endl;
    // }
    // b2Body *body = getBody();
    // if (!body)
    //     return;
    // for (b2Fixture *f = body->GetFixtureList(); f; f = f->GetNext())
    // {
    //     if (f->GetType() != b2Shape::e_polygon)
    //         continue;

    //     b2PolygonShape *poly = static_cast<b2PolygonShape *>(f->GetShape());
    //     int32 count = poly->m_count;

    //     std::vector<Vector2> points(count);
    //     for (int32 i = 0; i < count; ++i)
    //     {
    //         Vec2Adapter worldPoint(body->GetWorldPoint(poly->m_vertices[i]));
    //         points[i] = worldPoint.toPixels();
    //     }

    //     // Draw loop
    //     for (int i = 0; i < count; ++i)
    //     {
    //         DrawLineV(points[i], points[(i + 1) % count], RED);
    //     }
    //}
    // behavior->onDraw(dt);
}

void Block::updateCollision(GameObject *other, int type)
{
    Mario *player = dynamic_cast<Mario *>(other);
    if (!player)
        return;
    behavior->reactToCollision(player, type);
}

void Block::createBody(b2World *world)
{
    float posX = pos.toMeters().x;
    float posY = pos.toMeters().y;
    float halfWidth = size.getHalfWidth();
    float halfHeight = size.getHalfHeight();

    b2BodyDef bodyDef;
    bodyDef.fixedRotation = true;
    bodyDef.type = b2_staticBody;

    bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
    bodyDef.position.Set(posX, posY);
    body = world->CreateBody(&bodyDef);

    // Main box collider
    b2PolygonShape boxShape;
    boxShape.SetAsBox(halfWidth, halfHeight);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.friction = 0.8f;
    fixtureDef.density = 1.0f;
    if (!isSolid)
    {
        fixtureDef.isSensor = true;
        fixtureDef.filter.categoryBits = CATEGORY_NOTSOLID;
        fixtureDef.filter.maskBits = CATEGORY_CHARACTER_MAIN | CATEGORY_CHARACTER_SENSOR; // Detect the character's main body
    }
    else
    {
        fixtureDef.filter.categoryBits = CATEGORY_SOLID;                                  // Solid block
        fixtureDef.filter.maskBits = CATEGORY_CHARACTER_MAIN | CATEGORY_CHARACTER_SENSOR; // Detect the character's main body and sensors
    }
    b2Fixture *fixtureMain = body->CreateFixture(&fixtureDef);

    if (isSolid)
    {
        // 1. Top sensor
        b2PolygonShape topSensorShape;
        topSensorShape.SetAsBox(halfWidth * 0.9f, 2.0f / PPM, b2Vec2(0, -halfHeight + 2 / PPM), 0);

        b2FixtureDef topFixture;
        topFixture.shape = &topSensorShape;
        topFixture.isSensor = true;
        topFixture.filter.categoryBits = CATEGORY_SOLID;                                  // Solid block
        topFixture.filter.maskBits = CATEGORY_CHARACTER_MAIN | CATEGORY_CHARACTER_SENSOR; // Detect the character's main body and sensors
        topFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::TOP);
        body->CreateFixture(&topFixture);

        // 2. Bottom sensor
        b2PolygonShape bottomSensorShape;
        bottomSensorShape.SetAsBox(halfWidth * 0.825f, 2.0f / PPM, b2Vec2(0, halfHeight / 2), 0);

        b2FixtureDef bottomFixture;
        bottomFixture.shape = &bottomSensorShape;
        bottomFixture.isSensor = true;
        bottomFixture.filter.categoryBits = CATEGORY_SOLID;                                  // Solid block
        bottomFixture.filter.maskBits = CATEGORY_CHARACTER_MAIN | CATEGORY_CHARACTER_SENSOR; // Detect the character's main body and sensors
        bottomFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::BOTTOM);
        body->CreateFixture(&bottomFixture);
    }

    // // 3. Left side sensor
    // b2PolygonShape leftSensorShape;
    // leftSensorShape.SetAsBox(2.0f / PPM, halfHeight * 0.9f, b2Vec2(-halfWidth, 0), 0);

    // b2FixtureDef leftFixture;
    // leftFixture.shape = &leftSensorShape;
    // leftFixture.isSensor = true;
    // leftFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::LEFTSIDE);
    // body->CreateFixture(&leftFixture);

    // // 4. Right side sensor
    // b2PolygonShape rightSensorShape;
    // rightSensorShape.SetAsBox(2.0f / PPM, halfHeight * 0.9f, b2Vec2(halfWidth, 0), 0);

    // b2FixtureDef rightFixture;
    // rightFixture.shape = &rightSensorShape;
    // rightFixture.isSensor = true;
    // rightFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::RIGHTSIDE);
    // body->CreateFixture(&rightFixture);
}
