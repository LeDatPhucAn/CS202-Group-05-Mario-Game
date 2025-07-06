#pragma once

#include <vector>
#include "raylib.h"

// Rectangle cho AABB collision
struct Rect {
    Vector2 pos;   // góc trên-trái
    Vector2 size;  // width, height

    bool intersects(const Rect& other) const {
        return !(pos.x + size.x < other.pos.x
              || other.pos.x + other.size.x < pos.x
              || pos.y + size.y < other.pos.y
              || other.pos.y + other.size.y < pos.y);
    }

    operator Rectangle() const {
        return Rectangle{ pos.x, pos.y, size.x, size.y };
    }
};



class GameObject {
public:
    GameObject() = default;
    GameObject(const Vector2& _position, const Vector2& size)
        : pos(_position), size(size) {}

    virtual ~GameObject() = default;

    virtual void update() = 0;

    virtual void display() = 0;

    // Xử lý khi va chạm với một object khác
    // Dùng GameObject* thay vì shared_ptr
    virtual void updateCollision(GameObject* other) {
        // default: không làm gì

    }

    // Kiểm tra va chạm giữa hai GameObject
    bool checkCollision(const GameObject* other) {
        return getBounds().intersects(other->getBounds());
    }

    // Getter/Setter vị trí
    const Vector2& getPosition() const { return pos; }
    void setPosition(const Vector2& _pos) { pos = _pos; }

    // Getter/Setter Size Rectangle
    const Vector2& getSize() const { return size; }
    void setSize(const Vector2& s) { size = s; }

    Rect getBounds() const {
        return Rect{ pos, size };
    }

protected:
    //Góc trái trên của Rec
    Vector2 pos;

    //Size Rectangle
    Vector2 size;

};
