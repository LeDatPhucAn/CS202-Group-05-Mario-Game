#pragma once
#include "State.hpp"
#include <raylib.h>
#include <vector>
#include "UI.hpp"
using namespace std;

struct Sprite {
    vector<Vector2> StartEndFrames;
    vector<Rectangle> frameRecs;
	Texture2D texture;	
};

struct Movement {
	int speed = 0;
	Vector2 pos = { 0,0 };
	Vector2 velocity = { 0,0 };
	Vector2 acceleration = { 0,0 };
};

class State;
class Character {
protected:
    bool isGrounded = false;
    bool facingRight = true;
    Sprite sprite;
    Movement movement;
    State* currentState = nullptr;

public:
    friend class State;
    friend class IdleState;
    friend class WalkState;
    friend class JumpState;
    friend class FallState;
    struct Builder {
        Sprite sprite;
        Movement movement;
        State* state = nullptr;
        Builder& setSprite(vector<Vector2> StartEndFrames, json Json, Texture2D texture) {
            sprite.StartEndFrames = StartEndFrames;
            sprite.texture = texture;
            sprite.frameRecs = UI::JsonToRectangleVector(Json);
            return *this;
        }
        Builder& setMovement(int speed, Vector2 pos, Vector2 velocity, Vector2 acceleration) {
            movement.speed = speed;
            movement.pos = pos;
            movement.velocity = velocity;
            movement.acceleration = acceleration;
            return *this;
        }
        Builder& setState(State* initialState) {
            state = initialState;
            return *this;
        }
        Character build() {
            return Character(sprite, movement,state);
        }
    };
    Character() {}

    Character(const Sprite& _sprite, const Movement& _movement, State* _initialState)
        : sprite(_sprite), movement(_movement), currentState(_initialState) {
    }

    ~Character() {
        if (currentState) delete currentState;
    }

    void changeState(State* newState);
    void update();
    void draw();
};
