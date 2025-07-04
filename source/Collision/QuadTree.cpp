#include "Quadtree.hpp"

Quadtree::Quadtree(int pLevel, Rectangle pBounds)
    : level(pLevel), bounds(pBounds), nodes(4) {}

void Quadtree::clear() {
    objects.clear();
    for (auto& node : nodes)
        if (node) node->clear();
    nodes.clear();
    nodes.resize(4);
}

void Quadtree::split() {
    float subWidth = bounds.width / 2.0f;
    float subHeight = bounds.height / 2.0f;
    float x = bounds.x;
    float y = bounds.y;

    nodes[0] = std::make_unique<Quadtree>(level + 1, Rectangle{x + subWidth, y, subWidth, subHeight});
    nodes[1] = std::make_unique<Quadtree>(level + 1, Rectangle{x, y, subWidth, subHeight});
    nodes[2] = std::make_unique<Quadtree>(level + 1, Rectangle{x, y + subHeight, subWidth, subHeight});
    nodes[3] = std::make_unique<Quadtree>(level + 1, Rectangle{x + subWidth, y + subHeight, subWidth, subHeight});
}

int Quadtree::getIndex(const Rectangle& rect) {
    int index = -1;
    float midX = bounds.x + (bounds.width / 2.0f);
    float midY = bounds.y + (bounds.height / 2.0f);

    bool top = (rect.y < midY && rect.y + rect.height < midY);
    bool bottom = (rect.y > midY);

    if (rect.x < midX && rect.x + rect.width < midX) {
        if (top) index = 1;
        else if (bottom) index = 2;
    }
    else if (rect.x > midX) {
        if (top) index = 0;
        else if (bottom) index = 3;
    }

    return index;
}

void Quadtree::insert(const Rectangle& rect) {
    if (nodes[0]) {
        int index = getIndex(rect);
        if (index != -1) {
            nodes[index]->insert(rect);
            return;
        }
    }

    objects.push_back(rect);

    if (objects.size() > MAX_OBJECTS && level < MAX_LEVELS) {
        if (!nodes[0]) split();

        int i = 0;
        while (i < objects.size()) {
            int index = getIndex(objects[i]);
            if (index != -1) {
                Rectangle obj = objects[i];
                objects.erase(objects.begin() + i);
                nodes[index]->insert(obj);
            }
            else {
                i++;
            }
        }
    }
}

void Quadtree::retrieve(std::vector<Rectangle>& returnObjects, const Rectangle& rect) {
    int index = getIndex(rect);
    if (index != -1 && nodes[0]) {
        nodes[index]->retrieve(returnObjects, rect);
    }

    returnObjects.insert(returnObjects.end(), objects.begin(), objects.end());
}
