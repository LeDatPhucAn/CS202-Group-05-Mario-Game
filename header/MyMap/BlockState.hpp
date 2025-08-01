#pragma once
#include "Block.hpp"
#include "State.hpp"
class BlockState : public State{
protected:
    Block *block;
public:
    BlockState(int Type, Block *_block, int _delay)
        : State(Type, _block, _delay), block(_block) {}

    virtual void handleInput() override {
        // Handle input specific to block states
    }
};

class BlockIdleState : public BlockState {
public:
    BlockIdleState(Block *_block, int _delay = 5)
        : BlockState((int)blockStateType::IDLE, _block, _delay) {}

    void handleInput() override {
        // Handle input specific to idle block state
    }
};
class BlockActiveState : public BlockState {
public: 
    BlockActiveState(Block *_block, int _delay = 5)
        : BlockState((int)blockStateType::ACTIVE, _block, _delay) {}

    void handleInput() override {
        // Handle input specific to active block state
    }
};