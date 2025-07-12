#include "Character.hpp"
#include "GameObject.hpp"
void Character::changeState(State *newState)
{
    if (currentState)
        delete currentState;
    currentState = newState;
}
void Character::update()
{
    if (currentState)
    {
        currentState->updateState();
    }
}
void Character::display()
{
    if (currentState)
    {
        currentState->displayState();
    }
}


void Character::updateCollision(GameObject *other, int type)
{
    // if (type != NONE)
    // {
    //     cout << "CHARACTER: " << getBounds().x << " " << getBounds().y << " " << getBounds().width << " " << getBounds().height << "\n";
    //     cout << "BLOCK: " << other->getBounds().x << " " << other->getBounds().y << " " << other->getBounds().width << " " << other->getBounds().height << "\n";
    // }
    // switch (type)
    // {

    // case HEAD:
    //     cout << "Collision with TOP\n";
    //     break;
    // case FEET:
    //     cout << "Collision with BOTTOM\n";
    //     break;
    // case LEFTSIDE:
    //     cout << "Collision with LEFTSIDE\n";
    //     break;
    // case RIGHTSIDE:
    //     cout << "Collision with RIGHTSIDE\n";
    //     break;
    // }
    
    Rectangle bounds = getActualBounds();
    Vector2 posObj = other->getPosition();
    Vector2 sizeObj = other->getSize();
    
    if (Block *block = dynamic_cast<Block *>(other))
    {
        if (block->isSolid)
        {
            if (type == HEAD)
            {
                cout << type << "\n";
                pos.y = posObj.y - bounds.height;
                groundPosY = posObj.y;

                isGrounded = true;

                if (movement.velocity.y > 0)
                    movement.velocity.y = 0;

                maxHeight = posObj.y - maxJumpHeight - bounds.height;
            }
            else if (type == FEET) // Jumping up (hit ceiling)
            {
                cout << type << "\n";
                pos.y = posObj.y + sizeObj.y;
                movement.velocity.y = fallGravity * GetFrameTime();
            }
            else if (type == LEFTSIDE) // hitting left wall
            {   
                cout << type << "\n";
                pos.x = posObj.x - bounds.width;
            }
            else if (type == RIGHTSIDE) // hitting right wall
            {
                cout << type << "\n";
                pos.x = posObj.x + sizeObj.x;
            }
        }
    }
}

Rectangle Character::getBounds() const
{
    if (!currentState)
    {
        cerr << "Error: getBounds called on null currentState\n";
        return {0, 0, 0, 0};
    }
    return currentState->Bounds();
}
Rectangle Character::getActualBounds() const
{
    if (!currentState)
    {
        cerr << "Error: getBounds called on null currentState\n";
        return {0, 0, 0, 0};
    }
    return currentState->ActualBounds();
}
Rectangle Character::getFeet() const
{
    if (!currentState)
    {
        cerr << "Error: getFeet called on null currentState\n";
        return {0, 0, 0, 0};
    }
    return currentState->Feet();
}
Rectangle Character::getHead() const
{
    if (!currentState)
    {
        cerr << "Error: getHead called on null currentState\n";
        return {0, 0, 0, 0};
    }
    return currentState->Head();
}
Rectangle Character::getLeft() const
{
    if (!currentState)
    {
        cerr << "Error: getLeftSide called on null currentState\n";

        return {0, 0, 0, 0};
    }
    return currentState->LeftSide();
}
Rectangle Character::getRight() const
{
    if (!currentState)
    {
        cerr << "Error: getRightSide called on null currentState\n";

        return {0, 0, 0, 0};
    }
    return currentState->RightSide();
}
