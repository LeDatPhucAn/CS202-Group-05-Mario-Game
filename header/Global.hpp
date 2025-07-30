#pragma once
#include <iostream>
#include <vector>
#include <string>
using namespace std;

enum class SceneMode {
    GAME,
    MENU,
    PAUSE,
    WIN
};

const float screenWidth = 1600;
const float screenHeight = 800;

//Đọc từ file json của Map
const float TileSize = 16.f;

//đọc từ file json của Map
const int WorldHeightCNT = 14;
const int WorldWidthCNT = 28;

const float WorldHeight = WorldHeightCNT*TileSize;
const float WorldWidth = WorldWidthCNT*TileSize;

