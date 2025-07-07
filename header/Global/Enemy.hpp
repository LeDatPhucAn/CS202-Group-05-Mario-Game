#pragma once
#include "Character.hpp"

class Enemy : public Character {
public:
    Enemy() : Character() {}

    Enemy(const Sprite& _sprite, const Movement& _movement, State* _initialState, Vector2 _pos)
        : Character(_sprite, _movement, _initialState, _pos) {}
    Enemy(const Enemy& other) : Character(other) {
    }
public:
    struct Builder {
        Sprite sprite;
        Movement movement;
        State* state = nullptr;
        Vector2 pos = {100, 0}; //GetStarting Point from Map

        Builder& setFrames(stateType type, int start, int end) {
            sprite.StartEndFrames[type] = { start, end };
            return *this;
        }

        Builder& setJsonAndTexture(const std::string& name) {
            sprite.texture = UI::textureMap[name];
            sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap[name]);
            return *this;
        }

        Builder& setSpeed(int speed) {
            movement.speed = speed;
            return *this;
        }

        Builder& setPos(Vector2 _pos) {
            pos = _pos;
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

        Builder& setState(State* initialState) {
            state = initialState;
            return *this;
        }

        Enemy build() {
            return Enemy(sprite, movement, state, pos);
        }
    };
};
