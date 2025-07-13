#include "CollisionManager.hpp"

void CollisionManager::init(MyMap *map, vector<Character *> characters)
{
    if (map)
    {
        quadTree.setBounds(map->getMapBounds());
        for (auto &block : map->tileBlocks)
        {
            quadTree.insert(block);
            allObjects.push_back(block);
        }

        // Add characters to the collision manager
        for (Character *character : characters)
        {
            quadTree.insert(character);
            allObjects.push_back(character);
        }

        cout << "QuadTree initialized with " << allObjects.size() << " objects.\n";
    }
}

void CollisionManager::ManageCollision()
{

    for (GameObject *gameObject : allObjects)
    {
        vector<GameObject *> nearbyObjects;
        quadTree.retrieve(nearbyObjects, gameObject);

        for (GameObject *other : nearbyObjects)
        {
            int type = gameObject->checkCollision(other);
            if (type)
                gameObject->updateCollision(other, type);
                // other->updateCollision(gameObject, type);
        }
    }

    
}