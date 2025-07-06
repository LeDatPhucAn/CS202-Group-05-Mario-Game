#pragma once
#include "Character.hpp"
#include "MyMap.hpp"
#include "Scene.hpp"
#include "raylib-tileson.h"
#include "Enemy.hpp"

class Game : public Scene {
protected:
    Character Mario;
    Enemy Goomba;

    Character* MarioPointer = nullptr;
    
    std::unordered_map<std::string, std::string> mapPaths;
    string current_Map;
    MyMap curMap;
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
    
    ~Game() = default;

};

