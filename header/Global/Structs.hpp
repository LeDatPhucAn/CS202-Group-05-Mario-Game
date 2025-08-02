#pragma once

#include <raylib.h>
#include <raymath.h>
#include <vector>

using namespace std;

const float screenWidth = 1600;
const float screenHeight = 800;

// Đọc từ file json của Map
const float TileSize = 16.f;

// đọc từ file json của Map
const int WorldHeightCNT = 14;
const int WorldWidthCNT = 28;

const float WorldHeight = WorldHeightCNT * TileSize;
const float WorldWidth = WorldWidthCNT * TileSize;

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
    FALL,
    JUMP
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
enum class projectileStateType
{
    MOVE,
    STOP
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