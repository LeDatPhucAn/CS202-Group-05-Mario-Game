#pragma once
#include "Scene.hpp"
#include "Button.hpp"
#include "UI.hpp"

class PreGame : public Scene
{
private:
    Texture2D backgroundTexture;
    Texture2D logoTexture;
    
    std::vector<TextBox*> buttons;
    int selectedButton = 0;
    
    // Animation variables
    float logoScale = 1.0f;
    float logoScaleDirection = 1.0f;
    float animationTimer = 0.0f;
    
    void initializeButtons();
    void handleInput();
    void updateAnimations();

public:
    PreGame(SceneManager* _manager);
    ~PreGame();
    
    void updateScene() override;
    void displayScene() override;
};