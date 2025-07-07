#pragma once

#include <vector>
#include "raylib.h"

//Là một hình chữ nhật, mô tả mọi Object trong Game.
class GameObject {
public:
    GameObject() = default;
    GameObject(const Vector2& _position, const Vector2& size)
        : pos(_position), size(size) {}

    virtual ~GameObject() = default;

    virtual void update() = 0;

    virtual void display() = 0;

    // Xử lý khi va chạm với một object khác
    virtual void updateCollision(GameObject* other) {
    }

    // Kiểm tra va chạm giữa hai GameObject
    bool checkCollision(const GameObject* other) {
        return CheckCollisionRecs(this->getBounds(), other->getBounds());
    }

    // Getter/Setter vị trí
    const Vector2& getPosition() const { return pos; }
    void setPosition(const Vector2& _pos) { pos = _pos; }

    // Getter/Setter Size Rectangle
    const Vector2& getSize() const { return size; }
    void setSize(const Vector2& s) { size = s; }

    virtual Rectangle getBounds() const {
        return {pos.x, pos.y, size.x,size.y};
    }

    virtual Rectangle getFeet() const { return {0,0,0,0}; }
    virtual Rectangle getHead() const { return {0,0,0,0}; }
    virtual Rectangle getLeftSide() const { return {0,0,0,0}; }
    virtual Rectangle getRightSide() const { return {0,0,0,0}; }

protected:
    //Góc trái trên của Rec
    Vector2 pos;
    //Size Rectangle
    Vector2 size;

};


