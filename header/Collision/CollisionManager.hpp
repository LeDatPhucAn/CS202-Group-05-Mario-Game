#pragma once
#include "QuadTree.hpp"
#include "GameObject.hpp"
#include "MyMap.hpp"
#include "raylib.h"
#include "Mario.hpp"
#include "Enemy.hpp"
class CollisionManager
{
private:
    QuadTree quadTree;
    vector<GameObject *> allObjects;
    vector<GameObject *> dynamicObjects;
public:
    CollisionManager() = default;

    void init(MyMap *map, Mario* mario,  vector<Enemy*> enemies);
    void ManageCollision();
};