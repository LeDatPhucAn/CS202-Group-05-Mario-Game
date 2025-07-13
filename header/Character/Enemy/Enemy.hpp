#pragma once
#include "Character.hpp"
#include "UI.hpp" 
#include "MarioState.hpp"
class Enemy : public Character {
public:
    bool beCleared = false;

    Enemy();
    Enemy(const Sprite& _sprite, const Movement& _movement, State* _initialState, Vector2 _pos);
    Enemy(const Enemy& other);

    void updateCollision(GameObject* other, int type) override;

public:
    struct Builder {
        Sprite sprite;
        Movement movement;
        State* state = nullptr;
        Vector2 pos = {100, 0};

        Builder& setFrames(stateType type, int start, int end);
        Builder& setJsonAndTexture(const std::string& name);
        Builder& setSpeed(int speed);
        Builder& setPos(Vector2 _pos);
        Builder& setVelocity(Vector2 velocity);
        Builder& setAcceleration(Vector2 acceleration);
        Builder& setState(State* initialState);
        Enemy build();
    };
};

class Goomba : public Enemy {
public:
    Goomba() : Enemy() {
        this->sprite.StartEndFrames[IDLE] = {0, 0};
        this->sprite.StartEndFrames[WALK] = {0, 1};
        this->sprite.StartEndFrames[JUMP] = {0, 0};
        this->sprite.StartEndFrames[FALL] = {0, 1};
        this->sprite.StartEndFrames[DEAD] = {2, 2};
        this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Enemies2D"]);
        this->sprite.texture = UI::textureMap["Enemies2D"];
    }
    Goomba(const Sprite& _sprite, const Movement& _movement, State* _initialState, Vector2 _pos)
        : Enemy(_sprite, _movement, _initialState, _pos) {}
    Goomba(const Goomba& other) : Enemy(other) {}

    void updateCollision(GameObject* other, int type) override;
};

class Koopa : public Enemy {
private:
    int timesHit = 0;
public:
    Koopa() : Enemy() {
        this->sprite.StartEndFrames[IDLE] = {7, 7};
        this->sprite.StartEndFrames[WALK] = {3, 4};
        this->sprite.StartEndFrames[JUMP] = {3, 4};
        this->sprite.StartEndFrames[FALL] = {3, 4};
        this->sprite.StartEndFrames[RUN] = {7, 7};
        this->sprite.StartEndFrames[DEAD] = {7, 7};
        this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Enemies2D"]);
        this->sprite.texture = UI::textureMap["Enemies2D"];
    }
    Koopa(const Sprite& _sprite, const Movement& _movement, State* _initialState, Vector2 _pos)
        : Enemy(_sprite, _movement, _initialState, _pos) {}
    Koopa(const Koopa& other) : Enemy(other) {}

    void updateCollision(GameObject* other, int type) override;
};