#pragma once
#include <vector>
#include <memory>
#include "raylib.h"
#include "GameObject.hpp"
class QuadTree
{
private:
    const static int MAX_OBJECTS = 6;
    const static int MAX_LEVELS = 5;

    int level;
    Rectangle bounds;
    std::vector<GameObject*> objects;
    std::vector<std::unique_ptr<QuadTree>> nodes;

public:
    QuadTree() : level(0), bounds({0, 0, 0, 0}), nodes(4) {}
    QuadTree(int pLevel, Rectangle pBounds);
    ~QuadTree() { clear(); }
    void clear();
    void split();
    int getIndex(GameObject*rect);
    void insert(GameObject* rect);
    void retrieve(std::vector<GameObject*> &returnObjects, GameObject* rect);
    void setBounds(Rectangle pBounds);
    void resetDynamic();
};
