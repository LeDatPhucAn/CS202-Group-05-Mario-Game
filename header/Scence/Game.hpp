#pragma once
#include "Character.hpp"
#include "MyMap.hpp"
#include "Scene.hpp"
#include "raylib-tileson.h"

class Game : public Scene {
protected:
    Character Mario;
    string current_Map;
    MyMap map;
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

