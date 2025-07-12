#pragma once

#include <raylib.h>
#include <raymath.h>
#include <vector>

using namespace std;

struct StartEndFrame
{
    int start = 0;
    int end = 0;
};
struct Sprite
{
    vector<StartEndFrame> StartEndFrames = vector<StartEndFrame>(15);
    vector<Rectangle> frameRecs;
    Texture2D texture;
};

struct Movement
{
    int speed = 0;
    Vector2 velocity = {0, 0};
    Vector2 acceleration = {0, 0};
};

enum MarioForm
{
    SMALL = 0,
    BIG = 1,
    FIRE,
    FORM_COUNT
};
enum stateType
{
    IDLE,
    WALK,
    RUN,
    JUMP,
    FALL,
    SKID,
    CROUCH,
    GROW,
    UNGROW,
    DEAD
};

enum Direction
{
    LEFT = -1,
    BRUH,
    RIGHT = 1,
};