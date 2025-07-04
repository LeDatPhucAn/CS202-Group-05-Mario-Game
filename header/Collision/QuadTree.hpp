#pragma once
#include <vector>
#include <memory>
#include "raylib.h"

class Quadtree {
private:
    const static int MAX_OBJECTS = 4;
    const static int MAX_LEVELS = 5;

    int level;
    Rectangle bounds;
    std::vector<Rectangle> objects;
    std::vector<std::unique_ptr<Quadtree>> nodes;

public:
    Quadtree(int pLevel, Rectangle pBounds);

    void clear();
    void split();
    int getIndex(const Rectangle& rect);
    void insert(const Rectangle& rect);
    void retrieve(std::vector<Rectangle>& returnObjects, const Rectangle& rect);
};
