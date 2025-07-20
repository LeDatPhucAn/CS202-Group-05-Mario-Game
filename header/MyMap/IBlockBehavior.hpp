#pragma once
#include "raylib.h"
#include "GameObject.hpp"
#include <iostream>

class Block;
class Player;

struct IBlockBehavior {
    IBlockBehavior(Block* block) : block(block) {}

    virtual ~IBlockBehavior() { delete block; block = nullptr; };
    virtual void reactToCollision(GameObject* p, int type) {

    };
    virtual void updateFrame(float dt) { }
    virtual void onDraw(float dt) {};

    virtual void printType() const {

    }

    Block* block = nullptr; // Reference to the Block instance
};

class BrickBehavior : public IBlockBehavior {
public:
    BrickBehavior(Block* block) : IBlockBehavior(block) {}

    
    void printType() const override {

    }
    void reactToCollision(GameObject* p, int type) override;
    void updateFrame(float dt) override;
    void onDraw(float dt) override;
private:
};


class QuestionBehavior : public IBlockBehavior {
public:
    QuestionBehavior(Block* block) : IBlockBehavior(block) {}
    void printType() const override {

    }
    void reactToCollision(GameObject* p, int type) override;
    void updateFrame(float dt) override;
    //void onDraw(float dt) override;

};


class GroundBehavior : public IBlockBehavior {
public:
    GroundBehavior(Block* block) : IBlockBehavior(block) {}
    void printType() const override {

    }
    void reactToCollision(GameObject* p, int type) override;
    //void updateFrame(float dt) override;
    //void onDraw(float dt) override;
};

struct FactoryIBlockBehavior {
    static IBlockBehavior* create(const std::string& type, Block* block) {
        if (type == "BrickBlock") return new BrickBehavior(block);
        if (type == "QuestionBlock") return new QuestionBehavior(block);
        if (type == "GroundBlock") return new GroundBehavior(block);
        // Add more behaviors as needed
        return new IBlockBehavior(block); // Default behavior
    }
};
