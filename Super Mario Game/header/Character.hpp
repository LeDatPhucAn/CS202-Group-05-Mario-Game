#pragma once
#include "State.hpp"
#include <raylib.h>
#include <vector>
#include "UI.hpp"
using namespace std;

struct StartEndFrame {
    int start = 0;
    int end = 0;
};
struct Sprite {
    vector<StartEndFrame> StartEndFrames = vector<StartEndFrame>(10);
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
    struct Builder {
        Sprite sprite;
        Movement movement;
        State* state = nullptr;

        // --- Sprite setters ---
        Builder& setIdleFrames(int start, int end) {
            sprite.StartEndFrames[IDLE] = { start,end };
            return *this;
        }

        Builder& setWalkFrames(int start, int end) {
            sprite.StartEndFrames[WALK] = { start,end };
            return *this;
        }

        Builder& setJumpFrames(int start, int end) {
            sprite.StartEndFrames[JUMP] = { start,end };
            return *this;
        }

        Builder& setFallFrames(int start, int end) {
            sprite.StartEndFrames[FALL] = { start,end };
            return *this;
        }

        Builder& setJsonAndTexture(string name) {
            sprite.texture = UI::textureMap[name];
            sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap[name]);
            return *this;
        }

        // --- Movement setters ---
        Builder& setSpeed(int speed) {
            movement.speed = speed;
            return *this;
        }

        Builder& setPos(Vector2 pos) {
            movement.pos = pos;
            return *this;
        }

        Builder& setVelocity(Vector2 velocity) {
            movement.velocity = velocity;
            return *this;
        }

        Builder& setAcceleration(Vector2 acceleration) {
            movement.acceleration = acceleration;
            return *this;
        }

        // --- State ---
        Builder& setState(State* initialState) {
            state = initialState;
            return *this;
        }

        // --- Final build ---
        Character build() {
            return Character(sprite, movement, state);
        }

    };
    
public:
    friend class State;
    friend class IdleState;
    friend class WalkState;
    friend class JumpState;
    friend class FallState;

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
