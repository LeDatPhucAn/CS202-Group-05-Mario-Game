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
    vector<GameObject *> allObjects;

public:
    CollisionManager() = default;

    void init(MyMap *map, vector<Character *> characters);
    void ManageCollision();
};