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

enum MarioForm
{
    SMALL = 0,
    BIG = 1,
    FIRE,
    FORM_COUNT
};

enum class marioStateType
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
    DEAD,
    FLY
};
enum class enemyStateType
{
    IDLE,
    WALK,
    RUN,
    DEAD,
    FLY,
    FALL
};
enum class blockStateType
{
    IDLE,
    ACTIVE,
    BREAKABLE,
    QUESTION,
    PIPE,
    FLAGPOLE,
    INACTIVE
};
enum class itemStateType
{
    EMERGE,
    IDLE,
    FALL,
    MOVE,
    JUMP
};

enum Direction
{
    LEFT = -1,
    BRUH,
    RIGHT = 1,
};

enum CollisionType
{
    NONE,
    TOP,       // TOP là đầu của 1 object
    BOTTOM,    // BOTTOM là phần dưới của 1 object
    LEFTSIDE,  // LEFTSIDE là phần bên trái của 1 object
    RIGHTSIDE, // RIGHTSIDE là phần bên phải của 1 object
};