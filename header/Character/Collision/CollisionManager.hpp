#pragma once
#include "QuadTree.hpp"
#include "GameObject.hpp"
#include "MyMap.hpp"
#include "Character.hpp"
#include "raylib.h"
class CollisionManager
{
private:
    const float maxJumpHeight = 72.0f;
    const float jumpGravity = 1056.25f; // While holding jump
    const float fallGravity = 1462.5f;  // Letting go / falling
    const float fallSpeedCap = 240.0f;


    float maxHeight = 72.0f;
    QuadTree quadTree;
    bool NotItself(const Rectangle &other, const Rectangle &another)
    {
        return !(other.x == another.x && other.y == another.y &&
                 other.width == another.width && other.height == another.height);
    }

public:
    CollisionManager() = default;

    void init(MyMap *map, Character *character);
    void ResolveCollision(Character* character, const Rectangle &B);
    void ManageCollision(Character *character, float deltaTime);
};