// #pragma once
// #include "Character.hpp"
// #include "MarioState.hpp"

// // Forward declare state classes
// class State;
// class EnemyState;

// class Enemy : public Character
// {
// public:
//     bool beCleared = false;

//     Enemy();
//     Enemy(const Sprite &_sprite, State *_initialState, Vector2 _pos);
//     Enemy(const Enemy &other);

//     // Pure virtual function forces Goomba and Koopa to provide their own implementation.
//     virtual void updateCollision(GameObject *other, int type) = 0;

// public:
//     // The Builder is kept for use in Game.cpp
//     struct Builder
//     {
//         Sprite sprite;
//         State *state = nullptr;
//         Vector2 pos = {100, 0};

//         Builder &setFrames(stateType type, int start, int end);
//         Builder &setJsonAndTexture(const std::string &name);
//         Builder &setPos(Vector2 _pos);
//         Builder &setState(State *initialState);
//         // The build() method is specific to each enemy type, so it's removed from the base.
//     };
// };

// class Goomba : public Enemy
// {
// public:
//     Goomba();
//     Goomba(const Builder &builder); // Add a constructor that takes a builder

//     void updateCollision(GameObject *other, int type) override;
// };

// class Koopa : public Enemy
// {
// public:
//     Koopa();
//     Koopa(const Builder &builder); // Add a constructor that takes a builder

//     void updateCollision(GameObject *other, int type) override;
// };