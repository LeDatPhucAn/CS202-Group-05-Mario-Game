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
    vector<StartEndFrame> StartEndFrames = vector<StartEndFrame>(15);
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
    Direction direction = RIGHT;
    Sprite sprite;
    Movement movement;
    State* currentState = nullptr;
    MarioForm form = SMALL;
public:
    friend class State;
    friend class RunState;
    friend class IdleState;
    friend class WalkState;
    friend class JumpState;
    friend class FallState;
    friend class SkidState;
    friend class GrowState;
    friend class UnGrowState;
    Character() {}

    Character(const Sprite& _sprite, const Movement& _movement, State* _initialState)
        : sprite(_sprite), movement(_movement), currentState(_initialState) {
    }

    ~Character() {
        if (currentState) delete currentState;
    }
    Character(const Character& other) {
        sprite = other.sprite;
        movement = other.movement;
        isGrounded = other.isGrounded;
        direction = other.direction;
        currentState = new IdleState(this);
    }
    
    Character& operator=(const Character& other) {
        if (this == &other)return *this;
        // Clean up existing state
        if (currentState) delete currentState;

        sprite = other.sprite;
        movement = other.movement;
        isGrounded = other.isGrounded;
        direction = other.direction;

        currentState = new IdleState(this);  
        return *this;
    }
    void changeForm(MarioForm form) {
        switch (form) {
        case SMALL:
            sprite.StartEndFrames[IDLE] = { 0, 0 };
            sprite.StartEndFrames[JUMP] = { 5, 5 };
            sprite.StartEndFrames[FALL] = { 5, 5 };
            sprite.StartEndFrames[SKID] = { 4, 4 };
            sprite.StartEndFrames[WALK] = { 1, 3 };
            sprite.StartEndFrames[RUN] = { 1, 3 };
            sprite.StartEndFrames[GROW] = { 44, 50 };
            sprite.StartEndFrames[UNGROW] = { 0, 0 };
            break;
        case BIG:
            sprite.StartEndFrames[IDLE] = { 13, 13 };
            sprite.StartEndFrames[WALK] = { 16, 14 };
            sprite.StartEndFrames[JUMP] = { 18, 18 };
            sprite.StartEndFrames[FALL] = { 18, 18 };
            sprite.StartEndFrames[SKID] = { 17, 17 };
            sprite.StartEndFrames[RUN] = { 16, 14 };
            sprite.StartEndFrames[GROW] = { 28, 28 };
            sprite.StartEndFrames[UNGROW] = { 50, 44 };
            break;
        case FIRE:
            sprite.StartEndFrames[IDLE] = { 28, 28 };
            sprite.StartEndFrames[WALK] = { 30, 28 };
            sprite.StartEndFrames[JUMP] = { 34, 34 };
            sprite.StartEndFrames[FALL] = { 34, 34 };
            sprite.StartEndFrames[SKID] = { 33, 33 };
            sprite.StartEndFrames[RUN] = { 30, 28 };
            sprite.StartEndFrames[GROW] = { 28, 28 };
            sprite.StartEndFrames[UNGROW] = { 13, 13 };
            break;
        default:
            break;
        }
	}
    void changeState(State* newState);
    void update();
    void draw();

public:
    struct Builder {
        Sprite sprite;
        Movement movement;
        State* state = nullptr;

        // --- Sprite setters ---
        Builder& setFrames(stateType type, int start, int end) {
            sprite.StartEndFrames[type] = { start,end };
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

};
