#pragma once
#include "raylib.h"
#include "GameObject.hpp"
#include <iostream>
#include "Vec2Adapter.hpp"
class Block;
class Player;

struct IBlockBehavior
{
    float timeSinceHit = 0.0f;
    const float bounceVel = 3.5f;
    Vec2Adapter prePos;
    IBlockBehavior(Block *block);

    int direction = 1;

    virtual void throwMushroom(int direction = 1);
    virtual void throwStar(int direction = 1);

    virtual void setForBounce();
    virtual void setNoBounce();
    virtual void makeBlockBounce(float dt);
    virtual ~IBlockBehavior() {};
    virtual void reactToCollision(GameObject *p, int type) {

    };

    virtual void updateFrame(float dt) {}
    virtual void onDraw(float dt) {};

    Block *block = nullptr; // Reference to the Block instance
};

class BrickBehavior : public IBlockBehavior
{
public:
    BrickBehavior(Block *block) : IBlockBehavior(block) {}

    void reactToCollision(GameObject *p, int type) override;
    void updateFrame(float dt) override;
    void onDraw(float dt) override;

private:
};

class QuestionBehavior : public IBlockBehavior
{
private:
public:
    QuestionBehavior(Block *block) : IBlockBehavior(block) {}
    void reactToCollision(GameObject *p, int type) override;
    void updateFrame(float dt) override;
    // void onDraw(float dt) override;
};
class GroundBehavior : public IBlockBehavior
{
public:
    GroundBehavior(Block *block) : IBlockBehavior(block) {}
    void reactToCollision(GameObject *p, int type) override;
    // void updateFrame(float dt) override;
    // void onDraw(float dt) override;
};

class CoinBehavior : public IBlockBehavior
{
public:
    CoinBehavior(Block *block) : IBlockBehavior(block) {}
    void reactToCollision(GameObject *p, int type) override;

};


struct FactoryIBlockBehavior
{
    static IBlockBehavior *create(const std::string &type, Block *block)
    {
        if (type == "BrickBlock")
            return new BrickBehavior(block);
        if (type == "QuestionBlock")
            return new QuestionBehavior(block);
        if (type == "GroundBlock")
            return new GroundBehavior(block);
        if (type == "Coin")
            return new CoinBehavior(block);
            
        // Add more behaviors as needed
        return new IBlockBehavior(block); // Default behavior
    }
};
