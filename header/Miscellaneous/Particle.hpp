#pragma once

#include "GameObject.hpp"
#include "raylib.h"
#include "Movement.hpp"
#include <iostream>
using std::cout;

class Block;

class Particle : public GameObject {
public:
    Particle() = default;
    inline Particle(const Vector2& _pos, const Vector2& _size,
                    Texture2D _texture, const Rectangle& _srcRec,
                    const Vector2& initialVelocity, float lifetimeSeconds)
        : GameObject(_pos, _size)
        , texture(_texture)
        , srcRec(_srcRec)
        , isActive(true)
    {
        move.velocity     = initialVelocity;
        move.acceleration      = {0, 900.f};          
        maxLifetime  = lifetimeSeconds;
        age          = 0.0f;
    }

    static void spawnParticles(const Block& block, std::vector<Particle>& particles);


    // Update mỗi frame: physics + lifetime
 
    void update() override;

    // Vẽ nếu vẫn active
    void display(float dt);


    // Kiểm tra còn sống hay không
    inline bool active() const { return isActive; }

private:
    Texture2D texture;                        // chung sprite sheet
    Rectangle srcRec;                         // phần sprite của particle
    Movement  move;                           // giữ velocity, gravity, age, lifetime...
    bool      isActive = false;               // tắt khi hết lifetime
    float age;
    float maxLifetime;

    float rotationAngle = 0.0f;               // góc hiện tại (độ)
    float angularSpeed = 180.0f;   

};
