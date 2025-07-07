#include "CollisionManager.hpp"

void CollisionManager::init(MyMap *map, Character *_character)
{
    if (map)
    {
        int count = 0;
        quadTree.setBounds(map->getMapBounds());
        for (auto &block : map->objectBlocks)
        {
            quadTree.insert(block->getBounds());
            count++;
        }
        for (auto &block : map->tileBlocks)
        {
            quadTree.insert(block->getBounds());
            count++;
        }
        cout << "QuadTree initialized with " << count << " objects.\n";
    }
}

// void CollisionManager::ResolveCollision(Rectangle &A, const Rectangle &B)
// {

//     float A_right = A.x + A.width;
//     float A_bottom = A.y + A.height;
//     float B_right = B.x + B.width;
//     float B_bottom = B.y + B.height;

//     float overlapLeft = A_right - B.x;
//     float overlapRight = B_right - A.x;
//     float overlapTop = A_bottom - B.y;
//     float overlapBottom = B_bottom - A.y;

//     float minOverlapX = (overlapLeft < overlapRight) ? overlapLeft : -overlapRight;
//     float minOverlapY = (overlapTop < overlapBottom) ? overlapTop : -overlapBottom;

//     if (std::fabs(minOverlapX) < std::fabs(minOverlapY))
//         A.x -= minOverlapX;
//     else
//         A.y -= minOverlapY;
// }
void CollisionManager::ManageCollision(Character *character, float deltaTime)
{
    // apply gravity
    if (!character->isGrounded)
    {
        if (character->movement.velocity.y < 0)
        { // Going up
            if (IsKeyDown(KEY_UP) && character->pos.y > maxHeight)
            {
                character->movement.acceleration.y = jumpGravity;
            }
            else
            {
                character->movement.acceleration.y = fallGravity;
            }
        }
        else
        {
            character->movement.acceleration.y = fallGravity; // Going down
        }
    }
    else
    {
        character->movement.acceleration.y = 0;
    }
    character->movement.velocity += character->movement.acceleration * deltaTime;

    if (character->movement.velocity.y > fallSpeedCap)
        character->movement.velocity.y = fallSpeedCap;

    character->pos += character->movement.velocity * deltaTime;
    // cout << "pos: " << character->pos.x << " "
    //      << character->pos.y << "; velocity: "
    //      << character->movement.velocity.x << " "
    //      << character->movement.velocity.y << "; "
    //      << "pos change: " << character->movement.velocity.x * deltaTime << " "
    //      << character->movement.velocity.y * deltaTime << "\n";
    Rectangle bounds = character->getBounds();
    cout << "Before Collision: " << bounds.x << " " << bounds.y << " "
         << bounds.width << " " << bounds.height << " " << boolalpha << character->isGrounded << "\n";
    vector<Rectangle> nearbyObjects;
    quadTree.retrieve(nearbyObjects, bounds);
    character->isGrounded = false;
    for (const auto &rect : nearbyObjects)
    {
        if (NotItself(bounds, rect))
        {
            if (CheckCollisionRecs(character->getFeet(), rect))
            {
                cout<< "On Top Of: " << rect.x << " " << rect.y << " "
                     << rect.width << " " << rect.height << "\n";
                Rectangle feet = character->getFeet();
                cout << "Feet: " << feet.x << " " << feet.y << " "
                     << feet.width << " " << feet.height << "\n";
                character->pos.y = rect.y - bounds.height;

                character->isGrounded = true;
                if (character->movement.velocity.y > 0)
                    character->movement.velocity.y = 0;
                maxHeight = rect.y - maxJumpHeight - bounds.height;
            }
            else if (CheckCollisionRecs(character->getHead(), rect)) // Jumping up (hit ceiling)
            {
                character->pos.y = rect.y + rect.height;
                character->movement.velocity.y = fallGravity * deltaTime;
            }
            else if (CheckCollisionRecs(character->getRightSide(), rect)) // hitting left wall
            {

                character->pos.x = rect.x - bounds.width;

                // if (character->movement.velocity.x > 0)
                    // character->movement.velocity.x = 0;
            }
            else if (CheckCollisionRecs(character->getLeftSide(), rect)) // hitting right wall
            {
                character->pos.x = rect.x + rect.width;
                // if (character->movement.velocity.x < 0)
                    // character->movement.velocity.x = 0;
            }
            else{
                Rectangle feet = character->getFeet();
                cout << "Feet: " << feet.x << " " << feet.y << " "
                     << feet.width << " " << feet.height << "\n";
            }
        }

        bounds = character->getBounds();
    }
    bounds = character->getBounds();

    if (!character->isGrounded)
    {
        cout << "Not Grounded After Collision: " << bounds.x << " " << bounds.y << " "
             << bounds.width << " " << bounds.height << "\n";
    }
    else
    {
        cout << "Grounded After Collision: " << bounds.x << " " << bounds.y << " "
             << bounds.width << " " << bounds.height << "\n";
    }
}
