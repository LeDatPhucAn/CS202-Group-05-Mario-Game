#pragma once
#include "Character.hpp"
#include "Mario.hpp"
#include "MyMap.hpp"
#include "Scene.hpp"
#include "raylib-tileson.h"
#include "Enemy.hpp"
#include "EnemyState.hpp"
class Game : public Scene
{
protected:
    Mario Mario;
    Goomba Goomba;
    Koopa Koopa;

    // Character* MarioPointer = nullptr;
    std::unordered_map<std::string, std::string> mapPaths;
    string current_Map;
    MyMap curMap;
    vector<Enemy *> enemies;

    // for box2d integration
    b2World *world = nullptr;
    const float fallGravity = 1462.5f;

public:

    // initialize
    Game();

    void init() override;

    // update
    void updateScene() override;
    void updateSceneInCamera(Camera2D cam) {};

    // display
    void displayScene() override;
    void displaySceneInCamera() override;

    ~Game();
};
