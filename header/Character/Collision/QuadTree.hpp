#pragma once
#include <vector>
#include <memory>
#include "raylib.h"

class QuadTree {
private:
    const static int MAX_OBJECTS = 4;
    const static int MAX_LEVELS = 5;

    int level;
    Rectangle bounds;
    std::vector<Rectangle> objects;
    std::vector<std::unique_ptr<QuadTree>> nodes;

public:
    QuadTree() : level(0), bounds({0, 0, 0, 0}), nodes(4) {}
    QuadTree(int pLevel, Rectangle pBounds);
    ~QuadTree() {clear();}
    void clear();
    void split();
    int getIndex(const Rectangle& rect);
    void insert(const Rectangle& rect);
    void retrieve(std::vector<Rectangle>& returnObjects, const Rectangle& rect);
    void setBounds(Rectangle pBounds);
};
