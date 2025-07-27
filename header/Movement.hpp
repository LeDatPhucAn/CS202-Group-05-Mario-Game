#pragma once
#include "raylib.h"

inline const Vector2 sum(const Vector2 a, const Vector2 b) {
    return {a.x + b.x, a.y + b.y};
}
inline const Vector2 product(const Vector2 a, float k) {
    return {a.x*k, a.y*k};
}

struct Movement {
public:

    inline void update(Vector2 &pos, float dt) {
        velocity = sum(velocity , product(acceleration, dt));
        pos = sum(pos , product(velocity,dt));

        if(pos.y > GroundY) 
            pos.y = GroundY;
    }

    inline void setVelocity(const Vector2& _vec) { velocity = _vec; }
    inline void setAcceleration(const Vector2& _acc) { acceleration = _acc; }
    inline void setGround(float _num) { GroundY = _num; }

    Vector2 velocity; // Current velocity
    Vector2 acceleration = {0 , 9.8f}; // Current acceleration
    float maxSpeed; // Maximum speed limit
    float friction; // Friction to apply when no input is 

    float GroundY = 24000; // Ground level Y 

};