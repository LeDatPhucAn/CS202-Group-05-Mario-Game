#pragma once
#include "Character.hpp"
#include "Mario.hpp"
#include "MyMap.hpp"
#include "Scene.hpp"
#include "raylib-tileson.h"
#include "Enemy.hpp"
#include "Goomba.hpp"
#include "Koopa.hpp"
#include "PiranhaPlant.hpp"
#include "Lakitu.hpp"
#include "Spiny.hpp"
#include "EnemyState.hpp"
#include "CollisionManager.hpp"
class Game : public Scene {
protected:
    Mario Mario;
    Goomba Goomba;
    Koopa Koopa;
    PiranhaPlant PiranhaPlant;
    Lakitu Lakitu;

    //Character* MarioPointer = nullptr;
    
    std::unordered_map<std::string, std::string> mapPaths;
    string current_Map;
    MyMap curMap;
    vector<Enemy*> enemies;
public:
    static CollisionManager collisionManager;
    
    // initialize
    Game();
    
    void init() override;
    void addEnemy(Enemy* newEnemy); 
    // update
    void updateScene() override;
    void updateSceneInCamera(Camera2D cam) {};

    // display
    void displayScene() override;
    void displaySceneInCamera() override;
    
    ~Game() = default;

};

