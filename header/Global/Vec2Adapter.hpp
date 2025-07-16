#pragma once
#include <box2d/box2d.h>

#define PPM 16.0f // PIXEL PER METER

class Vec2Adapter
{
private:
    Vector2 vec;

public:
    Vec2Adapter() : vec{0, 0} {}
    Vec2Adapter(Vector2 pixelVec) : vec(pixelVec) {}
    Vec2Adapter(float xPixels, float yPixels) : vec{xPixels, yPixels} {}
    Vec2Adapter(b2Vec2 meters) : vec{meters.x * PPM, meters.y * PPM} {}

    // Get Raylib Vector2 (in pixels)
    Vector2 toPixels() const { return vec; }

    // Get b2Vec2 (in meters)
    b2Vec2 toMeters() const { return {vec.x / PPM, vec.y / PPM}; }

    float getHalfWidth() const { return (vec.x * 0.5f) / PPM; }
    float getHalfHeight() const { return (vec.y * 0.5f) / PPM; }

    float x() const { return vec.x / PPM; }
    float y() const { return vec.y / PPM; }
};
