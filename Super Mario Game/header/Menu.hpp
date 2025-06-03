#pragma once
#include "Scene.hpp"
#include "SceneHandler.hpp"
#include "Button.hpp"
class SceneHandler;
class Menu : public Scene {
protected:
    static constexpr int spacing = 100;
    vector<RectButton*> RectButtons;
    SceneHandler* scenehandler = nullptr;
public:
    Menu() { init(); };
    Menu(SceneHandler* handler) : scenehandler(handler) { init(); }
    void init() override;

    void updateScene() override;
    void updateSceneInCamera(Camera2D cam) {};

    void resetAnimations() {};
    void displayScene() override;
    void displaySceneInCamera() {}
};