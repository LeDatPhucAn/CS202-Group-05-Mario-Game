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
};

class GameObject {
public:
    GameObject(const Vector2& position, const Vector2& size)
        : position(position), size(size) {}

    virtual ~GameObject() = default;

    // Cập nhật trạng thái (di chuyển, logic,…)
    virtual void update(float deltaTime = 0) = 0;

    // Vẽ lên màn hình
    virtual void display() const = 0;

    // Xử lý khi va chạm với một object khác
    // Dùng GameObject* thay vì shared_ptr
    virtual void onCollision(GameObject* other) {
        // default: không làm gì
    }

    // Kiểm tra va chạm giữa hai GameObject
    bool checkCollision(const GameObject& other) const {
        return getBounds().intersects(other.getBounds());
    }

    // Getter/Setter vị trí
    const Vector2& getPosition() const { return position; }
    void setPosition(const Vector2& pos) { position = pos; }

    // Getter/Setter Size Rectangle
    const Vector2& getSize() const { return size; }
    void setSize(const Vector2& s) { size = s; }

protected:
    //Góc trái trên của Rec
    Vector2 position;

    //Size Rectangle
    Vector2 size;

    Rect getBounds() const {
        return Rect{ position, size };
    }
};
