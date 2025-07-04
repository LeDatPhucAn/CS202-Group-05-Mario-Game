#pragma once

#include "GameObject.hpp"
#include "tileson.hpp"    // để dùng tson::Object

// Enum chứa loại item mà Question Block có thể chứa
enum class Contains { None = 0, Coin, Mushroom, FireFlower, Star, OneUp };

class Block : public GameObject {
public:
    // === Thông tin cơ bản ===
    int       gid;        // Global tile ID
    Texture2D texture;    // Texture atlas
    Rectangle srcRec;     // Phần hình trong atlas

    // === Flags game-specific ===
    bool      isSolid        = true;
    bool      isBreakable    = false;
    bool      isQuestion     = false;
    bool      isInvisible    = false;
    bool      isUsed         = false;
    Contains  contains       = Contains::None;
    bool      isPipeEntrance = false;
    bool      isFlagPole     = false;

    // Constructor thô (không đọc property)
    Block(int _gid, Vector2 _pos, Vector2 _size,
          Texture2D _tex, Rectangle _src)
      : GameObject(_pos, _size)
      , gid(_gid)
      , texture(_tex)
      , srcRec(_src)
    {}

    // Constructor tự động đọc tất cả property từ tson::Object
    Block(tson::Object &obj,
          Vector2 _pos, Vector2 _size,
          Texture2D _tex, Rectangle _src)
      : GameObject(_pos, _size)
      , gid(obj.getGid())
      , texture(_tex)
      , srcRec(_src)
    {
        
        // Dùng trực tiếp obj.get<T>(name, default) từ Tileson
        
        isSolid        = obj.get<bool>("isSolid");
        isBreakable    = obj.get<bool>("isBreakable");
        isQuestion     = obj.get<bool>("isQuestion");
        isInvisible    = obj.get<bool>("isInvisible");
        isUsed         = obj.get<bool>("isUsed");

        // containsItem trong Tiled lưu dưới dạng int
        {
            int iv = obj.get<int>("containsItem");
            contains = static_cast<Contains>(iv);
        }

        isPipeEntrance = obj.get<bool>("isPipeEntrance");
        isFlagPole     = obj.get<bool>("isFlagPole");
    }

    // Cập nhật logic (animation, v.v.)
    void update(float deltaTime) override {
        // ví dụ blink animation cho Question Block
    }

    // Vẽ block
    void display() const override {
        if (isInvisible && !isUsed) return;
        DrawTextureRec(texture, srcRec, getPosition(), WHITE);
    }

    // Xử lý va chạm (như với Mario)
    void onCollision(GameObject* other) override {
        // có thể kiểm tra hitFromBelow và gọi onHitFromBelow()
    }

    // Khi hit từ dưới
    bool onHitFromBelow() {
        if (isBreakable) {
            // spawn debris, xóa block
            return true;
        }
        if (isQuestion && !isUsed) {
            isUsed = true;
            // spawn item theo contains
            return true;
        }
        return false;
    }
};
