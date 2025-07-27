#pragma once
#include "State.hpp"
#include "Items.hpp"
#include "Structs.hpp"
class ItemState : public State
{
protected:
    Items *item = nullptr;

public:
    ItemState(stateType _type, Items *_item, int _delay)
        : State(_type, _item, _delay), item(_item) {}

    // Override the handleInput method for item-specific behavior
    void handleInput() = 0;
};

class ItemMoveState : public ItemState
{
public:
    ItemMoveState(Items *_item, int _delay = 5)
        : ItemState(ITEM_MOVE, _item, _delay) {}

    void handleInput() override
    {
        b2Vec2 vel = item->getBody()->GetLinearVelocity();
        vel.x = item->direction * fabs(20 / PPM); // Ensure correct direction
        item->getBody()->SetLinearVelocity(vel);
    }
};

class ItemEmergeState : public ItemState
{
public:
    ItemEmergeState(Items *_item, int _delay = 5)
        : ItemState(ITEM_EMERGE, _item, _delay) {}

    void handleInput() override
    {
        // emerge logic
        b2Vec2 vel = item->getBody()->GetLinearVelocity();
        item->getBody()->ApplyForceToCenter({0, -gameObject->getBody()->GetMass() * jumpGravity}, true);
    }
};
