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
    vector<StartEndFrame> StartEndFrames = vector<StartEndFrame>(30);
    vector<Rectangle> frameRecs;
    Texture2D texture;
};


enum class Mode 
{ 
    SINGLE, 
    DUAL 
};
enum class Difficulty 
{ 
    EASY, 
    HARD, 
    HARDCORE 
};
enum PlayerForm
{
    SMALL = 0,
    BIG = 1,
    FIRE,
    FORM_COUNT
};

enum class playerStateType
{
    IDLE,
    WALK,
    RUN,
    JUMP,
    SMALLJUMP,
    SQUISH_ENEMY,
    KICK_SHELL,
    FALL,
    SKID,
    CROUCH,
    GROW,
    UNGROW,
    DEAD,
    THROWFB,
    STAR
};
enum class movingObjectStateType
{
    IDLE,
    WALK,
    RUN,
    DEAD,
    FLY,
    FALL,
    JUMP,
    STOP,
    FB_MOVE,
    STAR_MOVE,
    THROW
};
enum class blockStateType
{
    IDLE,
    ACTIVE,
    BOUNCE,
    BREAK,
    SPAWNITEM,
    USED,
    GETCOIN
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

enum class marioVoice
{
    HURT,
    JUMP,
    LETSGO
};

enum class sceneType
{
    NONE,
    MENU,
    PRE_GAME,
    GAME,
    CHOOSE_LEVEL,
    PAUSE,
    SETTING,
    GAMEOVER,
    CONTROLS,
    STAR
};

enum class Action
{
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_RUN,
    BUTTON_THROW_FIREBALL
    // ... add more as needed
};