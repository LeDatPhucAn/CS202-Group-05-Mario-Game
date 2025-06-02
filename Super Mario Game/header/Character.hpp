#pragma once
#include "State.hpp"
#include <raylib.h>
#include <vector>
using namespace std;

class State;
class Character {
protected:
	int maxFrames = 0;
	int numStates = 0;
	int frameWidth = 0;
	int frameHeight = 0;
	int speed = 0;
	Vector2 velocity = { 0,0 };
	Vector2 pos = { 0,0 };
	State* currentState = nullptr;

	Texture2D sprite;
	vector<int> stateFrames;
public:

	friend class State;
	friend class IdleState;
	friend class WalkState;

	Character(){}
	Character(int _speed, int _numStates, int _maxFrames, Vector2 _velocity, Vector2 _pos, Texture2D _sprite, const vector<int>& _stateFrames)
		: numStates(_numStates), maxFrames(_maxFrames), speed(_speed), velocity(_velocity), pos(_pos), sprite(_sprite), stateFrames(_stateFrames) {
		frameHeight = sprite.height / numStates;
		frameWidth = sprite.width / maxFrames;
		currentState = new IdleState(stateFrames[IDLE], this);
	}
	~Character() {
		if (currentState) delete currentState;
	}
	void changeState(State* newState);

	void update();
	void draw();
};