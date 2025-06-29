#pragma once
#include "Scene.hpp"
#include "Program.hpp"
#include "Character.hpp"
#include "raylib-tileson.h"

class Game : public Scene {
protected:
    Character Mario;
    string current_Map;
    vector<Map> Maps;
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

