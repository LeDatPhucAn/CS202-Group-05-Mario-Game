#pragma once
#include "SceneManager.hpp"
#include "Button.hpp"
class SceneHandler : public UI {
private:
    vector<Button*>SceneButtons;
    vector<SceneManager*> scenes;
    Camera2D camera = { 
        {static_cast<float> (UI::screenWidth) / 2, static_cast<float> (UI::screenHeight) / 5},  
        { static_cast<float> (UI::screenWidth) / 2, 0 },
        0.0f,  
        0.0f
    };
protected:
public:
    SceneManager* currentSceneObject;
    static Vector2 mouseWorldPos;
    SceneHandler();
    ~SceneHandler();
    int getCurrentScene();
    void initButtons();
    void updateCamera();
    void changeScene(Scene newScene);
    void updateCurrentScene();
    void displayCurrentScene();
};
