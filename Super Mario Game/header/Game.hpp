#pragma once
#include "Scene.hpp"
#include "Program.hpp"
#include "Character.hpp"

class Game : public Scene {
protected:
    Character Mario;
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

