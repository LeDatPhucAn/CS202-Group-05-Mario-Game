#pragma once

#include "GameObject.hpp"
#include "tileson.hpp"

// Enum chứa loại item mà Question Block có thể chứa
enum class Contains
{
    None = 0,
    Coin,
    Mushroom,
    FireFlower,
    Star,
    OneUp
};

// abstract class Block
class Block : public GameObject
{
public:
    int gid;           // Global tile ID
    Texture2D texture; // Texture trong sprite
    Rectangle srcRec;  // Khu vực cắt trong sprite

    bool isSolid = true;                // Có thể đi xuyên k (Mario đứng lên, va chạm)
    bool isBreakable = false;           // Có thể phá vỡ được (bricks, question block)
    bool isQuestion = false;            // Là ô dấu hỏi? hay ko
    bool isInvisible = false;           // true thì không vẽ (ví dụ Coin đã ăn rồi thì bật true)
    bool isUsed = false;                // Đã chạm vào chưa (Coin)
    //Contains contains = Contains::None; // Trong ô question chứa những thứ gì (Coin, Mushroom, Star,..)
    //bool isPipeEntrance = false;        // Đánh dấu nếu ống nước có đường bí mật
    bool isFlagPole = false;            // Đánh dấu cây cờ

    Block(int _gid, Vector2 _pos, Vector2 _size,
          Texture2D _tex, Rectangle _src);

    // Constructor tự động đọc tất cả property từ tson::Object
    Block(tson::Object &obj,
          Vector2 _pos, Vector2 _size,
          Texture2D _tex, Rectangle _src);

    void update() override;

    // Vẽ block
    void display() override;

    void updateCollision(GameObject *other, int type) override;
};

class GroundBlock : public Block {
public:
	GroundBlock(int _gid, Vector2 _pos, Vector2 _size,
          Texture2D _tex, Rectangle _src);

	GroundBlock(tson::Object &obj,
          Vector2 _pos, Vector2 _size,
          Texture2D _tex, Rectangle _src);
};