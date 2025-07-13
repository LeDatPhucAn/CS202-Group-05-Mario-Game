#include "QuadTree.hpp"
#include <iostream>
QuadTree::QuadTree(int pLevel, Rectangle pBounds)
    : level(pLevel), bounds(pBounds), nodes(4) {}

void QuadTree::clear()
{
    objects.clear();
    for (auto &node : nodes)
        if (node)
            node->clear();
    nodes.clear();
    nodes.resize(4);
}
void QuadTree::resetDynamic()
{
    // Remove all dynamic objects from this node
    objects.erase(
        std::remove_if(objects.begin(), objects.end(), [](GameObject *obj)
                       { return !obj->isStatic; }),
        objects.end());

    // Recurse into child nodes if they exist
    for (auto &node : nodes)
    {
        if (node)
            node->resetDynamic();
    }
}

void QuadTree::split()
{
    float subWidth = bounds.width / 2.0f;
    float subHeight = bounds.height / 2.0f;
    float x = bounds.x;
    float y = bounds.y;

    nodes[0] = std::make_unique<QuadTree>(level + 1, Rectangle{x + subWidth, y, subWidth, subHeight});
    nodes[1] = std::make_unique<QuadTree>(level + 1, Rectangle{x, y, subWidth, subHeight});
    nodes[2] = std::make_unique<QuadTree>(level + 1, Rectangle{x, y + subHeight, subWidth, subHeight});
    nodes[3] = std::make_unique<QuadTree>(level + 1, Rectangle{x + subWidth, y + subHeight, subWidth, subHeight});
}

int QuadTree::getIndex(GameObject *rect)
{
    Vector2 pos = rect->getPosition();
    Vector2 size = rect->getSize();
    float verticalMidpoint = bounds.x + bounds.width / 2.0f;
    float horizontalMidpoint = bounds.y + bounds.height / 2.0f;

    // Object can completely fit within the top quadrants
    bool top = (pos.y + size.y) < horizontalMidpoint;
    // Object can completely fit within the bottom quadrants
    bool bottom = pos.y > horizontalMidpoint;

    // Object can completely fit within the left quadrants
    bool left = (pos.x + size.x) < verticalMidpoint;
    // Object can completely fit within the right quadrants
    bool right = pos.x > verticalMidpoint;

    if (left)
    {
        if (top)
            return 1; // top-left
        else if (bottom)
            return 2; // bottom-left
    }
    else if (right)
    {
        if (top)
            return 0; // top-right
        else if (bottom)
            return 3; // bottom-right
    }

    return -1; // Doesn't fully fit in any quadrant
}

void QuadTree::insert(GameObject *rect)
{
    if (nodes[0] != nullptr)
    {
        int index = getIndex(rect);
        if (index != -1)
        {
            nodes[index]->insert(rect);
            return;
        }
    }

    objects.push_back(rect);

    if (objects.size() > MAX_OBJECTS && level < MAX_LEVELS)
    {
        if (nodes[0] == nullptr)
            split();

        auto it = objects.begin();
        while (it != objects.end())
        {
            int index = getIndex(*it);
            if (index != -1)
            {
                nodes[index]->insert(*it);
                it = objects.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

void QuadTree::retrieve(std::vector<GameObject *> &returnObjects, GameObject *rect)
{
    if (nodes[0])
    {
        for (int i = 0; i < 4; ++i)
        {
            if (CheckCollisionRecs(rect->getBounds(), nodes[i]->bounds))
            {
                nodes[i]->retrieve(returnObjects, rect);
            }
        }
    }

    // Add current node's objects
    returnObjects.insert(returnObjects.end(), objects.begin(), objects.end());
}

void QuadTree::setBounds(Rectangle pBounds)
{
    bounds = pBounds;
}
