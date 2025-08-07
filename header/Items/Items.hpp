#pragma once
#include "GameObject.hpp"
#include "Block.hpp"
#include "Structs.hpp"
#include "Movement.hpp"

class Items : public GameObject
{
public:
    void setFrame(itemStateType type, int start, int end)
    {
        this->sprite.StartEndFrames[(int)type] = {start, end};
    }

	// void createBody(b2World* world) override;
	// displaying and animation
	void appear(float dt, Block *owner);
	void test_appear();
	virtual void update() = 0;
	void display();

protected:
};