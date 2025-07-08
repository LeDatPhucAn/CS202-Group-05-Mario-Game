#pragma once
#include "QuadTree.hpp"
#include "GameObject.hpp"
#include "MyMap.hpp"
#include "Character.hpp"
#include "raylib.h"

class CollisionManager
{
private:
    QuadTree quadTree;

public:
    CollisionManager() = default;

    void init(MyMap *map);
    void ManageCollision(Character *character);
};