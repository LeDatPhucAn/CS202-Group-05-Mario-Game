#pragma once
#include "Scene.hpp"
#include "Button.hpp"

class Program;
class Menu : public Scene {
    //vector<Button> buttons;
public:
    Menu(SceneManager* _manager) : Scene(_manager) {}
    void updateScene();
    void displayScene();
};

