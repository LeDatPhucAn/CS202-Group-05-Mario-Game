#pragma once
#include "Scene.hpp"
#include "Button.hpp"
class SceneHandler : public UI {
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
    SceneHandler();
    ~SceneHandler();
    int getCurrentScene();
    void initButtons();
    void updateCamera();
    void changeScene(sceneType newScene);
    void updateCurrentScene();
    void displayCurrentScene();
};
