#pragma once
#include "SceneManager.hpp"
#include "SceneHandler.hpp"
#include "Button.hpp"
#include "Character.hpp"
class Game : public SceneManager {
protected:
    Character Mario;
public:
    Game();
    void init() override;
    void initButtons() override;
    void updateButtonPositions() {};

    void updateScene() override;
    void updateSceneInCamera(Camera2D cam) {};
    void resetAnimations() {};

    void displayScene() override;
    void displaySceneInCamera() override;
};