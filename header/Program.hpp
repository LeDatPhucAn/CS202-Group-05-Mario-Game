#pragma once
#include "Scene.hpp"
#include "raylib-tileson.h"
#include "Character.hpp"

class Character;
class Button;

class Program {
private:
    vector<Button*>SceneButtons;
    vector<Scene*> scenes;
    Camera2D camera = { 
        {static_cast<float> (UI::screenWidth) / 2, static_cast<float> (UI::screenHeight) / 5},  
        { static_cast<float> (UI::screenWidth) / 2, 0 },
        0.0f,  
        0.0f
    };

public:
    Scene* currentSceneObject;
    static Vector2 mouseWorldPos;
    
    // Basic methods 
    Program();
    void updateCurrentScene();
    void displayCurrentScene();
    void run();

    ~Program();

    //Scene
    int getCurrentScene();
    void changeScene(sceneType newScene);
    
    void initButtons();
    void updateCamera();
};
