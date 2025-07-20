#pragma once

#include "GameObject.hpp"
#include "tileson.hpp"

#include "IBlockBehavior.hpp"
#include <iostream>
#include <memory>
using namespace std;

struct TSInfo { int firstgid, columns, margin, spacing; Vector2 tileSize; };

// Enum chứa loại item mà Question Block có thể chứa
enum class Contains { None = 0, Coin, Mushroom, FireFlower, Star, OneUp };


class Block : public GameObject {

      friend class BrickBehavior;
      friend class QuestionBehavior;
      friend class GroundBehavior;

public:
      //Animation Components
      int       gid;        // Global tile ID
      Texture2D texture;    // Texture trong sprite
      Rectangle srcRec;     // Khu vực cắt trong sprite

      vector<Rectangle> srcRecs; //Load srcRec
      float duration = 0.0f;
      float aniTimer = 0.0f;
      int aniIndex = 0;

      // // Block Properties for Behavior
      bool      isSolid        = false;    //Có thể đi xuyên k (Mario đứng lên, va chạm)
      bool      isBreakable    = false;   //Có thể phá vỡ được (bricks, question block)
      bool      isQuestion     = false;   //Là ô dấu hỏi? hay ko
      bool      isInvisible    = false;   //true thì không vẽ (ví dụ Coin đã ăn rồi thì bật true)
      bool      isUsed         = false;   //Đã chạm vào chưa (Coin)
      Contains  contains       = Contains::None;  //Trong ô question chứa những thứ gì (Coin, Mushroom, Star,..)
      bool      isPipeEntrance = false;   //Đánh dấu nếu ống nước có đường bí mật
      bool      isFlagPole     = false;   //Đánh dấu cây cờ
      
      shared_ptr<IBlockBehavior> behavior;

      bool needDeletion = false; // Đánh dấu để xóa block nếu cần
      Color color = WHITE;

      Block(int _gid, Vector2 _pos, Vector2 _size,
            Texture2D _tex, Rectangle _src);

      Block(tson::Tile* inforTile, Vector2 _pos, Vector2 _size,
                  Texture2D _tex, Rectangle _src, const TSInfo* tsi);

      Block(tson::Object &obj,
            Vector2 _pos, Vector2 _size,
            Texture2D _tex, Rectangle _src);

      void update() override;
      void display() override;


      void updateCollision(GameObject* other, int type) override;

      
    void createBody(b2World *world) override
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
    body->CreateFixture(&fixtureDef);

    // // 1. Top sensor
    // b2PolygonShape topSensorShape;
    // topSensorShape.SetAsBox(halfWidth * 0.9f, 2.0f / PPM, b2Vec2(0, -halfHeight), 0);

    // b2FixtureDef topFixture;
    // topFixture.shape = &topSensorShape;
    // topFixture.isSensor = true;
    // topFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::TOP);
    // body->CreateFixture(&topFixture);

    // // 2. Bottom sensor
    // b2PolygonShape bottomSensorShape;
    // bottomSensorShape.SetAsBox(halfWidth * 0.9f, 2.0f / PPM, b2Vec2(0, halfHeight), 0);

    // b2FixtureDef bottomFixture;
    // bottomFixture.shape = &bottomSensorShape;
    // bottomFixture.isSensor = true;
    // bottomFixture.userData.pointer = static_cast<uintptr_t>(CollisionType::BOTTOM);
    // body->CreateFixture(&bottomFixture);

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


protected:
      //Physics Components
      void handleInput(float dt);
      void applyPhysics(float dt);
            // Physics & movement state
      Vector2 velocity;
      //static constexpr float speed     = 200.0f;  // pixels per second
      static constexpr float jumpForce = 200.0f;  // initial jump velocity
      static constexpr float gravity   = 900.0f;  // gravity acceleration
      static constexpr float maxHeight   = -8.0f;  
      bool onGround = true;
      //static constexpr int GroundY = 240;

      bool isJumping = false;
      Vector2 prePos;
};