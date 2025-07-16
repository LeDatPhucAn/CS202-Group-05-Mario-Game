#pragma once

#include <vector>
#include "raylib.h"
#include <algorithm>
#include <cmath>
using namespace std;

enum CollisionType
{
    NONE,
    HEAD,      // A HEAD B là A đứng trên B
    FEET,      // A nhảy lên đụng B
    LEFTSIDE,  // A ở bên trái B
    RIGHTSIDE, // A ở bên phải B
};

// Là một hình chữ nhật, mô tả mọi Object trong Game.
class GameObject
{
public:
    GameObject() = default;
    GameObject(const Vector2 &_position, const Vector2 &size)
        : pos(_position), size(size) {}

    virtual ~GameObject() = default;

    virtual void update() = 0;

    virtual void display() = 0;

    // Xử lý khi va chạm với một object khác
    virtual void updateCollision(GameObject *other, int type)
    {
    }

    // Kiểm tra va chạm giữa hai GameObject. Return enum CollisionType là Direction
    virtual int checkCollision(const GameObject *other)
    {
        if (this == other)
            return NONE; // Không va chạm với chính nó
        // Phải thêm epsilon chỉnh bounds vì frame Raylib không chính xác
        const float eps = -0.6f;

        Rectangle A = getBounds();
        A.x -= eps;
        A.y -= eps;
        A.width += 2 * eps;
        A.height += 2 * eps;

        Rectangle B = other->getBounds();
        B.x -= eps;
        B.y -= eps;
        B.width += 2 * eps;
        B.height += 2 * eps;
        if (CheckCollisionRecs(getFeet(), B))
        {
            return HEAD;
        }
        if (!CheckCollisionRecs(A, B))
            return NONE;

        float penLeft = fabsf(A.x + A.width - B.x),
              penRight = fabsf(B.x + B.width - A.x), // Đo độ sâu "chèn", ngược lại để dương
            penTop = fabsf(A.y + A.height - B.y),
              penBottom = fabsf(B.y + B.height - A.y);

        float m = min({penLeft, penRight, penTop, penBottom}); // abs hết cho lành

        if (m == penTop)
            return HEAD;
        if (m == penBottom)
            return FEET;
        if (m == penLeft)
            return LEFTSIDE;
        if (m == penRight)
            return RIGHTSIDE;

        return NONE;
    }

    const Vector2 &getPosition() const { return pos; }
    void setPosition(const Vector2 &_pos) { pos = _pos; }

    const Vector2 &getSize() const { return size; }
    void setSize(const Vector2 &s) { size = s; }

    virtual Rectangle getBounds() const
    {
        return {pos.x, pos.y, size.x, size.y};
    }

    const Vector2 getCenter() { return {pos.x + size.x / 2, pos.y + size.y / 2}; }
    float slice = 2;
    bool isStatic = true;
    virtual Rectangle getFeet() const { return {pos.x, pos.y + size.y - slice, size.x, slice}; }
    virtual Rectangle getHead() const { return {pos.x, pos.y, size.x, slice}; }
    virtual Rectangle getLeft() const { return {pos.x, pos.y, slice, size.y}; }
    virtual Rectangle getRight() const { return {pos.x + size.x - slice, pos.y, slice, size.y}; }

protected:
    // Góc trái trên của Rec
    Vector2 pos;
    // Size Rectangle
    Vector2 size;
};
