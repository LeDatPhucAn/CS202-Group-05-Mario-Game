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
    Rectangle bounds = getActualBounds();
    Vector2 posObj = other->getPosition();
    Vector2 sizeObj = other->getSize();

    if (Block *block = dynamic_cast<Block *>(other))
    {
        if (block->isSolid)
        {
            if (type == HEAD)
            {
                pos.y = posObj.y - bounds.height;
                groundPosY = posObj.y;

                isGrounded = true;

                if (movement.velocity.y > 0)
                    movement.velocity.y = 0;

                maxHeight = posObj.y - maxJumpHeight - bounds.height;
            }
            else if (type == FEET) // Jumping up (hit ceiling)
            {
                pos.y = posObj.y + sizeObj.y;
                movement.velocity.y = fallGravity * GetFrameTime();
            }
            else if (type == LEFTSIDE) // hitting left wall
            {
                pos.x = posObj.x - bounds.width;
            }
            else if (type == RIGHTSIDE) // hitting right wall
            {
                pos.x = posObj.x + sizeObj.x;
            }
        }
    }
}
int Character::checkCollision(const GameObject *other)
{
    if (this == other)
        return NONE; // Không va chạm với chính nó
    // Phải thêm epsilon chỉnh bounds vì frame Raylib không chính xác
    const float eps = -0.6f;

    Rectangle A = getBounds();
    A.x -= eps;
    A.y -= eps;
    A.width += 2 * eps;
    A.height += 2 * eps;

    Rectangle B = other->getBounds();
    B.x -= eps;
    B.y -= eps;
    B.width += 2 * eps;
    B.height += 2 * eps;
    // Rectangle circle = getFeet();
    if (CheckCollisionRecs(getFeet(), B))
    {
        return HEAD;
    }
    if (!CheckCollisionRecs(A, B))
        return NONE;

    float penLeft = fabsf(A.x + A.width - B.x),
          penRight = fabsf(B.x + B.width - A.x), // Đo độ sâu "chèn", ngược lại để dương
        penTop = fabsf(A.y + A.height - B.y),
          penBottom = fabsf(B.y + B.height - A.y);

    float m = min({penLeft, penRight, penTop, penBottom}); // abs hết cho lành

    if (m == penTop)
        return HEAD;
    if (m == penBottom)
        return FEET;
    if (m == penLeft)
        return LEFTSIDE;
    if (m == penRight)
        return RIGHTSIDE;

    return NONE;
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
