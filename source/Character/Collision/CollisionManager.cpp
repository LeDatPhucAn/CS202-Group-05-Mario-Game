#include "CollisionManager.hpp"

void CollisionManager::init(MyMap *map)
{
    if (map)
    {
        int count = 0;
        quadTree.setBounds(map->getMapBounds());
        for (auto &block : map->objectBlocks)
        {
            quadTree.insert(block);
            count++;
        }
        for (auto &block : map->tileBlocks)
        {
            quadTree.insert(block);
            count++;
        }
        cout << "QuadTree initialized with " << count << " objects.\n";
    }
}

void CollisionManager::ManageCollision(Character *character)
{
    vector<GameObject *> nearbyObjects;
    quadTree.retrieve(nearbyObjects, character);

    character->isGrounded = false;

    for (GameObject *gameObject : nearbyObjects)
    {
        character->updateCollision(gameObject);
        gameObject->updateCollision(character);
    }
}
