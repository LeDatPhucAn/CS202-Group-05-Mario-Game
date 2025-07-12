#pragma once
#include "Character.hpp"
#include "Mario.hpp"
#include "MyMap.hpp"
#include "Scene.hpp"
#include "raylib-tileson.h"
#include "Enemy.hpp"
#include "EnemyState.hpp"
#include "CollisionManager.hpp"
class Game : public Scene {
protected:
    Mario Mario;
    Goomba Goomba;
    Koopa Koopa;

    //Character* MarioPointer = nullptr;
    
    std::unordered_map<std::string, std::string> mapPaths;
    string current_Map;
    MyMap curMap;
    vector<Character*> characters;
public:
    static CollisionManager collisionManager;
    
    // initialize
    Game();
    
    void init() override;

    // update
    void updateScene() override;
    void updateSceneInCamera(Camera2D cam) {};

    // display
    void displayScene() override;
    void displaySceneInCamera() override;
    
    ~Game() = default;

};

