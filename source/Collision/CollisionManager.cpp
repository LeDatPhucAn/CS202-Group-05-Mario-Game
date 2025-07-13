#include "CollisionManager.hpp"

void CollisionManager::init(MyMap *map, Mario *mario, vector<Enemy *> enemies)
{
    quadTree.setBounds(map->getMapBounds());
    for (auto &block : map->tileBlocks)
    {
        quadTree.insert(block);
        allObjects.push_back(block);
    }

    // Add enemies to the collision manager
    for (Enemy *enemy : enemies)
    {
        enemy->isStatic = false;
        quadTree.insert(enemy);
        allObjects.push_back(enemy);
        dynamicObjects.push_back(enemy);
    }
    mario->isStatic = false;
    quadTree.insert(mario);
    allObjects.push_back(mario);
    dynamicObjects.push_back(mario);

    cout << "QuadTree initialized with " << allObjects.size() << " objects.\n";
}

void CollisionManager::ManageCollision()
{
    // reinsert the new positions of dynamic objects
    // quadTree.resetDynamic();
    // for (GameObject *dynamicObject : dynamicObjects)
    // {
    //     quadTree.insert(dynamicObject);
    // }

    for (GameObject *gameObject : allObjects)
    {
        vector<GameObject *> nearbyObjects;
        quadTree.retrieve(nearbyObjects, gameObject);

        for (GameObject *other : nearbyObjects)
        {
            int type = gameObject->checkCollision(other);
            if (type)
            {
                gameObject->updateCollision(other, type);
                // other->updateCollision(gameObject, type);
            }
        }
    }
}