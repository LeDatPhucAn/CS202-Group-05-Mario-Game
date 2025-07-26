#pragma once
#include "Scene.hpp"
#include "Button.hpp"
class Program;
class Menu : public Scene {
protected:
    static constexpr int spacing = 100;
    vector<RectButton*> RectButtons;
    Program* scenehandler = nullptr;
public:
    Menu() { init(); };
    Menu(Program* handler) : scenehandler(handler) { init(); }
    void init() override;

    void updateScene() override;
    void updateSceneInCamera(Camera2D cam) {};

    void resetAnimations() {};
    void displayScene() override;
    void displaySceneInCamera() {}
};