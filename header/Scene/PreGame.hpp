#pragma once
#include "Scene.hpp"
#include "Button.hpp"
#include "UI.hpp"

class PreGame : public Scene
{
private:
    Texture2D backgroundTexture;
    Texture2D marioTexture;
    Texture2D luigiTexture;
    Texture2D dualTexture;

    std::vector<TextBox*> difficultyButtons;  // Only for difficulty buttons now
    int selectedModeIndex = 0;  // 0=Mario, 1=Luigi, 2=Dual
    int selectedDifficultyIndex = 0;  // 0=Easy, 1=Hard, 2=Hardcore, 3=Play, 4=Back
    bool isSelectingMode = true;  // true=selecting mode images, false=selecting difficulty buttons
    
    // Animation variables
    float animationTimer = 0.0f;
    
    void initializeDifficultyButtons();
    void handleInput();
    void updateAnimations();

public:
    PreGame(SceneManager* _manager);
    ~PreGame();
    
    void updateScene() override;
    void displayScene() override;
};